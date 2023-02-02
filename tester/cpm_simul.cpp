#include "cpmmanager.h"
#include "conversationhistory.h"
#include "sessionhistory.h"
#include "filetransferhistory.h"
#include "groupstate.h"
#include "media.h"
#include "message.h"
#include "sessioninfo.h"

/*
int main() {
    
    try {
        CpmManager cpmManagerWrite("/Users/quentin/Desktop/CPM message storage/test/", "Simul");
        for (int i = 0; i<10; i++){
            shared_ptr<HistoryCpmObject> conversationHistory = cpmManagerWrite.createNewConversationHistory("CH"+to_string(i));
            shared_ptr<SessionHistory> sessionHistory = make_shared<SessionHistory>();
            conversationHistory->append(sessionHistory);
            for (int j = 0; j<10; j++){
                shared_ptr<Message> message = sessionHistory->createNew<Message>();
                message->headers.FROM = "quentin.monnier"+to_string(i)+"@gmail.com";
                message->headers.TO = "jehan.monnier"+to_string(i)+"@gmail.com";
                message->headers.CONTENT_TYPE = "text/plain; charset=utf-8";
                message->headers.DATE = "2022-01-18T13:40:00-08:0"+to_string(j);
                message->headers.subject = "simulation"+to_string(i);
                message->message_body = "challa ça marche\nc'est le msg n°"+to_string(j);
            }
            
        }
        cpmManagerWrite.write();
        
        CpmManager cpmManagerRead("/Users/quentin/Desktop/CPM message storage/test/", "Simul");
        cpmManagerRead.read();
        
    } catch(string const &e) {
            
        cout << e << endl;
        
    }
    
}*/

int main() {
    string defaultPath = "./testDirectory/";
    shared_ptr<CpmManager> cpmManager = make_shared<CpmManager>(defaultPath, "manager");
    list<uint32_t> cpmList;
    for (int i = 0; i<10; i++) {
        shared_ptr<ConversationHistory> CH = make_shared<ConversationHistory>("CH_"+to_string(i));
        uint32_t uid = cpmManager->append(CH);
        cpmList.push_back(uid);
    }
    shared_ptr<Message> message = make_shared<Message>();
    message->headers.FROM = "azertyuiop@tester.fr";
    message->headers.TO = "qwertyuiop@tester.co.uk";
    message->headers.CONTENT_TYPE = "text/plain; charset=utf-8";
    message->headers.DATE = "2022-01-18T13:40:00-08:00";
    message->headers.subject = "test";
    message->message_body = "msg n°1\ntest n°1";
    for (auto uid : cpmList){
        shared_ptr<StorageCpmObject> ch = dynamic_pointer_cast<StorageCpmObject>(cpmManager->fetchByUID(uid));
        shared_ptr<Message> message1 = make_shared<Message>(*message);
        ch->append(message1);
        shared_ptr<SessionHistory> sess = make_shared<SessionHistory>();
        sess->groupState->headers.GROUP_TYPE = "closed";
        sess->groupState->headers.LASTFOCUSSESSIONID = "da9274453@company.com";
        sess->groupState->headers.TIMESTAMP = "2012-06-13T16:39:57-05:00";
        sess->groupState->participants.add("bob", "tel:+16135551212");
        sess->groupState->participants.add("alice", "tel:+15195551212");
        sess->groupState->participants.add("bernie", "tel:+15145551212");
        sess->groupState->update();
        sess->groupState->participants.add("alicia", "tel:+16195551212");
        sess->groupState->headers.TIMESTAMP = "2012-04-13T16:39:57-05:00";
        sess->groupState->update();
        sess->groupState->headers.TIMESTAMP = "2012-07-13T16:51:57-05:00";
        sess->groupState->update();
        sess->groupState->headers.TIMESTAMP = "2012-07-13T16:51:57+05:00";
        sess->groupState->update();
        sess->groupState->headers.TIMESTAMP = "2012-07-13T16:51:57+03:00";
        sess->groupState->update();
        sess->sessionInfo->headers.FROM = "azertyuiop@tester.fr";
        sess->sessionInfo->headers.CONVERSATION_ID = "f81d4fae-7dec-11d0-a765-00a0c91e6bf6";
        sess->sessionInfo->headers.CONTRIBUTION_ID = "abcdef-1234-5678-90ab-cdef01234567";
        sess->sessionInfo->headers.CONTENT_TYPE = "multipart/related; boundary=cpm; type=”Application/X-CPM-Session”";
        sess->sessionInfo->headers.DATE = "2022-01-18T13:40:00-08:00";
        sess->sessionInfo->headers.subject = "test";
        sess->sessionInfo->xcpm.headers.TYPE = XCPM::PRE_DEFINED;
        sess->sessionInfo->xcpm.headers.INVITED_PARTICIPANTS = {"jdoe@machine.example", "sip:alice@example.com"};
        Media media;
        media.headers.TYPE = "audio/basic";
        media.headers.TRANSFER_ENCODING = "base64";
        media.headers.ID = "1234@example.com";
        media.sdp_parameters = "a=sendonly\nm=audio 49170 RTP/AVP 0 97";
        media.media_body = "... base64-encoded 8000 Hz single-channel mu-law-format audio data goes here ...";
        sess->sessionInfo->xcpm.addMedia(media);
        Media newMedia(media);
        newMedia.headers.ID = "5678@example.com";
        sess->sessionInfo->xcpm.addMedia(newMedia);
        Media newMedia2(media);
        newMedia2.headers.ID = "9876@example.com";
        sess->sessionInfo->xcpm.addMedia(newMedia2);
        ch->append(sess);
        sess->groupState->headers.TIMESTAMP = "2014-07-13T16:51:57-05:00";
        sess->groupState->update();
        shared_ptr<Message> message2 = make_shared<Message>(*message);
        sess->append(message2);
        shared_ptr<FileTransferHistory> ft = make_shared<FileTransferHistory>();
        ft->headers.FROM = "azertyuiop@tester.fr";
        ft->headers.CONVERSATION_ID = "f81d4fae-7dec-11d0-a765-00a0c91e6bf6";
        ft->headers.CONTRIBUTION_ID = "abcdef-1234-5678-90ab-cdef01234567";
        ft->headers.CONTENT_TYPE = "multipart/related; boundary=cpm; type=”Application/X-CPM-File-Transfer”";
        ft->headers.DATE = "2022-01-18T13:40:00-08:00";
        ft->headers.subject = "test";
        ft->xcpm.headers.TYPE = XCPM::PRE_DEFINED;
        ft->xcpm.headers.INVITED_PARTICIPANTS = {"jdoe@machine.example", "sip:alice@example.com"};
        ft->xcpm.addMedia(media);
        sess->append(ft);
        
    }
}

/*
int main() {
    string FROM = "";
    string from = "From";
    map<string*,string*> test;
    test[&from]=&FROM;
    cout << *test.find(&from)->second << endl;
    
    list<pair<string,string>> liste;
    liste.push_back(make_pair("erdf", "klopu"));
    test[&liste.rbegin()->first] = &liste.rbegin()->second;
    cout << *test.find(&liste.rbegin()->first)->second << endl;

    vector<string> vecto = {"3GFDFGDL3"};
    string *oui = &*vecto.rbegin();
    *vecto.rbegin() = "fdsgfsd";
    cout << *oui << endl;
    test[&*vecto.rbegin()] = oui;
    
    FROM = "eoruezio";
    cout << *test.find(&from)->second << endl;
    *test[&from] = "desdxc";
    cout << FROM << endl;
    
    map<string,string> content;
    content["blabla"] = "blublu";
    content["From"] = "bloblo";
    Headers headers;
    headers.setHeaders(content);
}*/
