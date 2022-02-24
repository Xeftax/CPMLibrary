#ifndef cpm_message_storage_test_h
#define cpm_message_storage_test_h
#include <cxxtest/TestSuite.h>
#include "cpmmanager.h"
#include "conversationhistory.h"
#include "sessionhistory.h"
#include "filetransferhistory.h"
#include "groupstate.h"
#include "media.h"
#include "message.h"
#include "sessioninfo.h"
#include "errors.h"

class MyTestSuite : public CxxTest::TestSuite
{
public:
    
    void testUIDtoHexConvertor(void) {
        
        CpmObject cpmObject;
        shared_ptr<StorageCpmObject> storageCpmObject = make_shared<StorageCpmObject>("name");
        cpmObject.mParent = storageCpmObject;
        string hexUid;
        pair<uint32_t,uint32_t> uids;
        
        /* uid min = 0 */
        TS_ASSERT_EQUALS(cpmObject.getUID(), 0)
        TS_ASSERT_EQUALS(cpmObject.getUIDValidity(), 0)
        hexUid = cpmObject.UIDstoHexString();
        uids = cpmObject.hexStringtoIntUIDs(hexUid);
        TS_ASSERT_EQUALS(hexUid, "0000000000000000")
        TS_ASSERT_EQUALS(uids.first, 0)
        TS_ASSERT_EQUALS(uids.second, 0)
        
        /* uid random */
        uint32_t uid1;
        uint32_t uid2;
        cpmObject.UID = uid1;
        storageCpmObject->UID = uid2;
        hexUid = cpmObject.UIDstoHexString();
        uids = cpmObject.hexStringtoIntUIDs(hexUid);
        TS_ASSERT_EQUALS(uids.first, uid2)
        TS_ASSERT_EQUALS(uids.second, uid1)
        
        
        /* uid max = 4294967295 */
        uid1 = 4294967295;
        uid2 = 4294967295;
        storageCpmObject->UID = uid1;
        cpmObject.UID = uid2;
        TS_ASSERT_EQUALS(cpmObject.getUID(), uid1)
        TS_ASSERT_EQUALS(cpmObject.getUIDValidity(), uid2)
        hexUid = cpmObject.UIDstoHexString();
        uids = cpmObject.hexStringtoIntUIDs(hexUid);
        TS_ASSERT_EQUALS(uids.first, uid1)
        TS_ASSERT_EQUALS(uids.second, uid2)
        
    }
        
    
    void testCpmObject(void) {
        
        /* init */
        CpmObject cpmObject;
        TS_ASSERT_EQUALS(cpmObject.getUID(), 0);
        TS_ASSERT_EQUALS(cpmObject.getUIDValidity(), 0);
        TS_ASSERT_EQUALS(cpmObject.getFolderName(), cpmObject.mUnknownString);
        TS_ASSERT_EQUALS(cpmObject.getObjectType(), CpmObject::objectType);
        TS_ASSERT_EQUALS(cpmObject.folderPathCheck("../tester"), "../tester/");
        TS_ASSERT_EQUALS(cpmObject.folderPathCheck("../tester/"), "../tester/");
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.folderPathCheck("./rdxkwdzgh");
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::WrongPath);
            throw e;
        });
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.cpmObjectValidity();
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        
        
        /* test  UIDs */
        uint32_t nUid = 3274896;
        cpmObject.UID = nUid;
        TS_ASSERT_EQUALS(cpmObject.getUID(), nUid);
        shared_ptr<StorageCpmObject> storageCpmObject = make_shared<StorageCpmObject>("name");
        cpmObject.mParent = storageCpmObject;
        storageCpmObject->UID = nUid;
        TS_ASSERT_EQUALS(cpmObject.getUIDValidity(), nUid);
        TS_ASSERT_EQUALS(cpmObject.getFolderName(), "name");
        TS_ASSERT_THROWS_NOTHING(cpmObject.cpmObjectValidity());
        
        /* test write-read */
        CpmObject unlinkedCpmObject;
        string defaultPath = "./testDirectory/";
        string path = defaultPath + storageCpmObject->UIDstoHexString()+"_"+cpmObject.getFolderName()+PATH_SEP;
        filesystem::remove_all(defaultPath);
        filesystem::create_directory(defaultPath);
        filesystem::create_directory(path);
        ofstream newFile1(path+cpmObject.UIDstoHexString());
        newFile1 << "Content-Type: Unknown/CPIM" << endl;
        newFile1 << endl << "1: 1" << endl << "2: 2" << endl << "3: 3" << endl;
        newFile1 << endl << 4 << endl << 5;
        newFile1.close();
        filesystem::create_directory(defaultPath+"wrongDirectory/");
        ofstream newFile2(defaultPath+"wrongDirectory/"+cpmObject.UIDstoHexString());
        newFile2 << "Content-Type: Unknown";
        newFile2.close();
        TS_ASSERT_THROWS_NOTHING(cpmObject.write(path));
        TS_ASSERT_THROWS_ANYTHING(try {unlinkedCpmObject.write(path);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.write(defaultPath+"wrongDirectory");
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::InconsistentPath);
            throw e;
        });
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.write("./dfsrpmrxc/"+path);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::WrongPath);
            throw e;
        });
        TS_ASSERT_THROWS_NOTHING(cpmObject.read(path + cpmObject.UIDstoHexString()));
        TS_ASSERT_THROWS_NOTHING(unlinkedCpmObject.read(path + cpmObject.UIDstoHexString()));
        for (int i = 1; i <= 3; i++) {
            TS_ASSERT_EQUALS(cpmObject.headersRead.find(to_string(i))->second,to_string(i));
            TS_ASSERT_EQUALS(unlinkedCpmObject.headersRead.find(to_string(i))->second,to_string(i));
        }
        TS_ASSERT_EQUALS(cpmObject.typeRead, cpmObject.getObjectType());
        TS_ASSERT_EQUALS(unlinkedCpmObject.typeRead, cpmObject.getObjectType());
        TS_ASSERT_EQUALS(cpmObject.bodyRead, "4\n5");
        TS_ASSERT_EQUALS(unlinkedCpmObject.bodyRead, "4\n5");
        unlinkedCpmObject.cleanCache();
        TS_ASSERT_EQUALS(unlinkedCpmObject.typeRead.empty(), true);
        TS_ASSERT_EQUALS(unlinkedCpmObject.bodyRead.empty(), true);
        TS_ASSERT_EQUALS(unlinkedCpmObject.headersRead.empty(), true);
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.read(defaultPath + cpmObject.UIDstoHexString());
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::WrongPath);
            throw e;
        });
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.read(defaultPath + "wrongDirectory/" + cpmObject.UIDstoHexString());
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::IncompatibleCpmObject);
            throw e;
        });
        
        /* equality test */
        shared_ptr<CpmObject> copyCpmObject = make_shared<CpmObject>(cpmObject);
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.isEqual(copyCpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::IDsUnequality);
            throw e;
        });
        copyCpmObject->UID = cpmObject.UID;
        copyCpmObject->mParent = cpmObject.mParent;
        TS_ASSERT_THROWS_NOTHING(cpmObject.isEqual(copyCpmObject))
        copyCpmObject->cleanCache();
        TS_ASSERT_THROWS_ANYTHING(try {cpmObject.isEqual(copyCpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::CacheUnequality);
            throw e;
        });
        copyCpmObject->UID = storageCpmObject->UID;
        copyCpmObject->mParent.reset();
        TS_ASSERT_THROWS_ANYTHING(try {copyCpmObject->isEqual(storageCpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::TypeUnequality);
            throw e;
        });
    }
    
    void testMessage(void) {
        
        /* init */
        shared_ptr<Message> message = make_shared<Message>();
        TS_ASSERT_EQUALS(Message::objectType, message->getObjectType())
        shared_ptr<StorageCpmObject> storageCpmObject = make_shared<StorageCpmObject>("name");
        message->mParent = storageCpmObject;
        TS_ASSERT_THROWS_ANYTHING(try {message->cpmObjectValidity();
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::IncompleteCpmObject);
            throw e;
        });
        
        /* headers */
        message->headers.FROM = "azertyuiop@tester.fr";
        message->headers.TO = "qwertyuiop@tester.co.uk";
        message->headers.CONTENT_TYPE = "text/plain; charset=utf-8";
        message->headers.DATE = "2022-01-18T13:40:00-08:00";
        TS_ASSERT_THROWS_NOTHING(message->cpmObjectValidity());
        message->headers.subject = "test";
        message->message_body = "msg n°1\ntest n°1";
        Message headerMessage;
        headerMessage.headers.setHeaders(message->headers.getMap());
        TS_ASSERT_EQUALS(message->headers.getMap(), headerMessage.headers.getMap())
        
        /* write-read */
        message->UID = 50432;
        string defaultPath = "./testDirectory/";
        string path = defaultPath+storageCpmObject->UIDstoHexString()+"_"+message->getFolderName()+PATH_SEP;
        filesystem::create_directory(path);
        TS_ASSERT_THROWS_NOTHING(message->write(path));
        shared_ptr<Message> copyMessage = make_shared<Message>(*message);
        TS_ASSERT_THROWS_ANYTHING(try {copyMessage->write(path);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        shared_ptr<Message> incompleteMessage = make_shared<Message>();
        incompleteMessage->mParent = storageCpmObject;
        TS_ASSERT_THROWS_ANYTHING(try {incompleteMessage->write(path);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::IncompleteCpmObject);
            throw e;
        });
        incompleteMessage->UID = message->UID;
        TS_ASSERT_THROWS_ANYTHING(try {message->isEqual(incompleteMessage);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::ContentUnequality);
            throw e;
        });
        TS_ASSERT_THROWS_NOTHING(incompleteMessage->read(path+incompleteMessage->UIDstoHexString()));
        TS_ASSERT_EQUALS(message->isEqual(incompleteMessage), true);
        TS_ASSERT_EQUALS(incompleteMessage->headersRead.empty(), true);
        TS_ASSERT_EQUALS(incompleteMessage->bodyRead.empty(), true);
    }
    
    void testStorageCpmObject(void) {
        /* init */
        shared_ptr<StorageCpmObject> storageCpmObject = make_shared<StorageCpmObject>("name");
        TS_ASSERT_EQUALS(storageCpmObject->getName(), "name");
        TS_ASSERT_EQUALS(storageCpmObject->getObjectType(), StorageCpmObject::objectType)
        TS_ASSERT_EQUALS(storageCpmObject->knownCPMtypeObjects.size(), 1)
        TS_ASSERT_EQUALS(storageCpmObject->knownCPMtypeObjects.front(), CpmObject::objectType)
        TS_ASSERT_THROWS_ANYTHING(try {storageCpmObject->getNextUID();
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        
        /* append-fetch */
        shared_ptr<CpmObject> cpmObject;
        TS_ASSERT_THROWS_ANYTHING(try {storageCpmObject->append(cpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        TS_ASSERT_THROWS_ANYTHING(try {storageCpmObject->createNew<Message>();
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        string defaultPath = "./testDirectory/";
        shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(defaultPath,"manager");
        storageCpmObject->mParent = cpmManager;
        TS_ASSERT_THROWS_ANYTHING(try {storageCpmObject->append(cpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), -1);
            throw e;
        });
        StorageCpmObject badStorageCpmObject("bad");
        badStorageCpmObject.mParent = cpmManager;
        TS_ASSERT_THROWS_ANYTHING(try {badStorageCpmObject.append(cpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), -1);
            throw e;
        });
        cpmObject = make_shared<CpmObject>();
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->append(cpmObject));
        TS_ASSERT_THROWS_ANYTHING(try {storageCpmObject->append(cpmObject);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::LinkedCpmObject);
            throw e;
        });
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->fetch<CpmObject>())
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->fetch<SessionHistory>())
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->fetch<Message>())
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->fetch<FileTransferHistory>())
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->fetch<Media>())
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->fetch<GroupState>())
        TS_ASSERT_EQUALS(storageCpmObject->fetch<CpmObject>().size(), 1)
        TS_ASSERT_EQUALS(storageCpmObject->fetch().size(), 1)
        
        /* write-read */
        badStorageCpmObject.mParent.reset();
        TS_ASSERT_THROWS_ANYTHING(try {badStorageCpmObject.write(defaultPath);
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::UnlinkedCpmObject);
            throw e;
        });
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->write(defaultPath))
        TS_ASSERT_THROWS_NOTHING(storageCpmObject->read(defaultPath+storageCpmObject->UIDstoHexString()+"_"+storageCpmObject->NAME))
    }
    
    void testCpmManager (void) {
        
        /* init */
        string defaultPath = "./testDirectory/";
        shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(defaultPath, "manager");
        TS_ASSERT_EQUALS(cpmManager->getPath(), defaultPath);
        TS_ASSERT_EQUALS(cpmManager->getObjectType(), CpmManager::objectType)
        TS_ASSERT_EQUALS(cpmManager->knownCPMtypeObjects.size(), 1)
        TS_ASSERT_EQUALS(cpmManager->knownCPMtypeObjects.front(), ConversationHistory::objectType)
        TS_ASSERT_EQUALS(cpmManager->getNextUID(), 101)
        TS_ASSERT_EQUALS(cpmManager->nextUID, cpmManager->getNextUID())
        
        /* append-fetch */
        TS_ASSERT_THROWS_NOTHING(cpmManager->createNew<ConversationHistory>("convHist"))
        
        //void removeConversationHistory(uint32_t uid);
        //void removeConversationHistory(string name);
        
        /* write-read */
        TS_ASSERT_THROWS_NOTHING(cpmManager->write())
        TS_ASSERT_THROWS_ANYTHING(try {cpmManager->write();
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::InconsistentPath);
            throw e;
        });
        TS_ASSERT_THROWS_ANYTHING(try {cpmManager->read();
        } catch(Errors const &e) {
            TS_ASSERT_EQUALS(e.getCode(), codeError::InvalidUID);
            throw e;
        });
            
    }
    
    void testConversationHistory (void) {}
    
    void testSimul (void) {
        /*try {
        
            CpmManager cpmManagerWrite("/Users/quentin/Desktop/CPM message storage/test/", "Simul");
            for (int i = 0; i<10; i++){
                shared_ptr<ConversationHistory> conversationHistory = cpmManagerWrite.createNewConversationHistory("CH"+to_string(i));
                shared_ptr<SessionHistory> sessionHistory = make_shared<SessionHistory>();
                conversationHistory->append(sessionHistory);
                //shared_ptr<GroupState> message = conversationHistory->createNew<GroupState>();
                for (int j = 0; j<10; j++){
                    shared_ptr<Message> message = sessionHistory->createNew<Message>();
                    message->headers.FROM = "quentin.monnier"+to_string(i)+"@gmail.com";
                    message->headers.TO = "jehan.monnier"+to_string(i)+"@gmail.com";
                    message->headers.CONTENT_TYPE = "text/plain; charset=utf-8";
                    message->headers.DATE = "2022-01-18T13:40:00-08:0"+to_string(j);
                    message->headers.subject = "simulation"+to_string(i);
                    message->MESSAGE_BODY = "challa ça marche\nc'est le msg n°"+to_string(j);
                }
                
            }
            cpmManagerWrite.write();
            
            CpmManager cpmManagerRead("/Users/quentin/Desktop/CPM message storage/test/", "Simul");
            cpmManagerRead.read();
            
            
        } catch(string const &e) {
                
            cout << e << endl;
            
        }*/
    }
    
};



#endif /* cpm_test_h */

