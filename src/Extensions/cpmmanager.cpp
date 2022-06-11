#include <filesystem>
#include "cpmmanager.h"
#include "conversationhistory.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "groupstate.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"


//LoggerPtr logger(Logger::getLogger("main"));

mutex CpmManager::mMutex;

CpmManager::CpmManager(string path, string name) : StorageCpmObject(name), defaultPath(folderPathCheck(path)) {
    //LOG4CXX_INFO (logger, "CpmManager [" << name << "] created.");
    nextUID = UID +1;
}

CpmManager::CpmManager(CpmManager &copy) : StorageCpmObject(copy), defaultPath(copy.defaultPath) {
    //LOG4CXX_INFO(logger, "CpmManager [" << copy.NAME << "] copied.");
    nextUID = UID +1;
}

CpmManager::~CpmManager() {
    ofstream newFile(getPath()+UIDstoHexString(make_pair(getUID(), nextUID)));
    newFile.close();
}

string CpmManager::getPath(){
    return defaultPath+NAME+PATH_SEP;
}

uint32_t CpmManager::getNextUID(){
    lock_guard<mutex> guard(mMutex);
    return nextUID++;
}

string CpmManager::UIDstoHexString(pair<uint32_t,uint32_t> uids){
    ostringstream stream;
    stream << setfill ('0') << setw(sizeof(uint32_t)*2) << hex << uids.first << setfill ('0') << setw(sizeof(uint32_t)*2) << hex << uids.second;
    return stream.str();
}

pair<uint32_t,uint32_t> CpmManager::hexStringtoIntUIDs(string hexuid){
    string uidValStr = hexuid.substr(0,8);
    string uidStr = hexuid.substr(8,16);
    uint32_t uidValidity = stoul(uidValStr, 0, 16);
    uint32_t uid = stoul(uidStr, 0, 16);
    return pair<uint32_t,uint32_t>(uidValidity,uid);
}

void CpmManager::scan() {
    StorageCpmObject::scan();
    if (nextUID==1 and not cpmObjectUIDList.empty()) {
        set<uint32_t>::iterator firstUID = cpmObjectUIDList.begin();
        nextUID = *firstUID;
        cpmObjectUIDList.erase(firstUID);
        filesystem::remove(getPath()+UIDstoHexString(make_pair(getUID(), *firstUID)));
    }
}

uint32_t CpmManager::append(shared_ptr<CpmObject> cpmObject) {
    if (nextUID==1) scan();
    return StorageCpmObject::append(cpmObject);
}

string CpmManager::write(shared_ptr<CpmObject> cpmObject){
    try {cpmObject->isComplete();} catch(Errors const &e) {throw(Errors(e,"error in writing a CpmObject"));}
    string path = cpmObject->getPath();
    if (dynamic_pointer_cast<StorageCpmObject>(cpmObject)) {
        filesystem::create_directory(path);
        shared_ptr<SessionHistory> ifSessionHistory = dynamic_pointer_cast<SessionHistory>(cpmObject);
        if (ifSessionHistory)
            write(ifSessionHistory->sessionInfo);
    } else {
        ofstream newFile(path);
        newFile << cpmObject->preview();
        newFile.close();
        try {
            cpmObject->checkWritingIntegrity();
        } catch (Errors const &e) {
            throw e;
            //if (e.getCode() == ContentUnequality) throw Errors(e,"error in checking the writing integrity of the object");
        }
    }
    return path;
}

shared_ptr<CpmObject> CpmManager::read(string path){
    const string except = "error while reading "+path;
    
    //for storage
    if (path.substr(path.length() -1) == PATH_SEP) path.pop_back();
    string name = path.substr(path.find_last_of(PATH_SEP) +1);
    if (name.find("_") != string::npos) {
        try {
            pair<uint32_t, uint32_t> uids = hexStringtoIntUIDs(name.substr(0, name.find("_")));
            uids.second = uids.second++;
            string childName = UIDstoHexString(uids);
            shared_ptr<SessionInfo> cpmObject = dynamic_pointer_cast<SessionInfo>(read(path + PATH_SEP + childName));
            if (cpmObject) {
                shared_ptr<SessionHistory> sessionHistory = make_shared<SessionHistory>();
                sessionHistory->sessionInfo = cpmObject;
                return sessionHistory;
            }
        } catch(Errors const &e) {
            if (e.getCode() != WrongPath) throw e;
            return make_shared<ConversationHistory>(name.substr(name.find("_") +1));
        }
    }
    
    
    
    //for data object
    ifstream newFile(path);
    if (not newFile.is_open()){
        throw Errors(WrongPath, except+" : unable to open the file");
    }
    
    string type;
    map<string,string> headers;
    ostringstream body;
    
    string lineContent;
    int lineIndex = 0;
    int fileSection = 0;
    while (getline(newFile,lineContent)){
        if (lineContent == "" and fileSection < 3) {
            ++fileSection;
        } else {
            switch (fileSection) {
                case 0: {
                    int firstHeaderSize = sizeof(HEADER_CONTENT_TYPE HEADER_SEP)/sizeof(char)-1;
                    if (lineIndex >= 0 and lineContent.substr(0,firstHeaderSize) != HEADER_CONTENT_TYPE HEADER_SEP) {
                        throw Errors(IncompatibleCpmObject, except+" : 1st line is wrong (bad syntax)");
                    }
                    type = lineContent.substr(firstHeaderSize);
                    break;
                }
                    
                case 1: {
                    size_t headerSep = lineContent.find(HEADER_SEP);
                    if (headerSep != string::npos) {
                        string header = lineContent.substr(0,headerSep);
                        string content = lineContent.substr(headerSep +sizeof(HEADER_SEP)/sizeof(char) -1);
                        headers.insert(make_pair(header,content));
                    } else {
                        //LOG4CXX_WARN(logger, "invalid header in " << path << "line" << lineIndex);
                    }
                    break;
                }
                    
                case 2:
                    body << lineContent;
                    ++fileSection;
                    break;
                    
                default:
                    body << endl << lineContent;
                    break;
            }
        }
        lineIndex++;
    }
    newFile.close();
    
    if (fileSection < 2) throw Errors(IncompatibleCpmObject, except+" : the layout of the file does not match with the layout of a CPM Object file");
    
    if (type == Message::objectType) return make_shared<Message>(headers, body.str());
    if (type == FileTransferHistory::objectType) return make_shared<FileTransferHistory>(headers, body.str());
    if (type == SessionInfo::objectType) return make_shared<SessionInfo>(headers, body.str());
    if (type == GroupState::objectType) return make_shared<GroupState>(headers, body.str());


    return shared_ptr<CpmObject>();
        
}

const string& CpmManager::getObjectType() {
    return objectType;
}

const vector<string>& CpmManager::getAllowedCpmObjectTypes() {
    return allowedCpmObjectTypes;
}
