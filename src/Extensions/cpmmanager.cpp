#include <filesystem>
#include "cpmmanager.h"
#include "conversationhistory.h"


mutex CpmManager::mMutex;
const shared_ptr<StorageCpmObject> CpmManager::cpmSeed = make_shared<StorageCpmObject>("seed");

CpmManager::CpmManager(string path, string name, uint32_t firstUID) : StorageCpmObject(name), defaultPath(folderPathCheck(path)) {
    
    UID = firstUID;
    nextUID = firstUID +1;
    mParent = cpmSeed;
    
}

CpmManager::CpmManager(CpmManager &copy, uint32_t firstUID) : StorageCpmObject(copy), defaultPath(copy.defaultPath) {
    UID = firstUID;
    nextUID = firstUID +1;
    mParent = cpmSeed;
}

string CpmManager::getPath(){
    return defaultPath;
}

uint32_t CpmManager::getNextUID(){
    lock_guard<mutex> guard(mMutex);
    return nextUID++;
}

/*
void CpmManager::removeConversationHistory(uint32_t uid){
    if (conversationHistoryList.erase(uid) == 0){
        throw ("no object found for uid : "+to_string(uid));
    };
}

void CpmManager::removeConversationHistory(string name){
    bool find = false;
    for (auto obj : conversationHistoryList) {
        if (obj.second->getFolderName() == name){
            conversationHistoryList.erase(obj.first);
            find = true;
            break;
        }
    }
    if (not find) throw ("no object found for the name : "+name);
}*/

void CpmManager::read(){
    StorageCpmObject::read(defaultPath+NAME);
}

void CpmManager::write(){
    try {
        folderPathCheck(defaultPath+NAME);
        throw Errors(InconsistentPath,"error in writing "+defaultPath+NAME+" : a directory with this name already exist");
    } catch(Errors const &e) {
        if (e.getCode() != WrongPath) throw e;
        StorageCpmObject::write(defaultPath);
    }
}

const string& CpmManager::getObjectType() {
    return objectType;
}

const vector<string>& CpmManager::getKnownCPMtypeObjects() {
    return knownCPMtypeObjects;
}
