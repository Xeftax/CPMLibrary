#ifndef CPMMANAGER_H
#define CPMMANAGER_H

#include <cstdint>
#include <string>
#include <list>
#include <map>
#include <mutex>
#include "storagecpmobject.h"

using namespace std;

class ConversationHistory;

template<typename CpmObj>
class MapByType : public map<uint32_t, shared_ptr<CpmObject> > {
    
    public :
    
    CpmObj UIDfetch(uint32_t uid){
        auto ptrCpmObjectIt = find(uid);
        if (ptrCpmObjectIt != end()) {
            return dynamic_pointer_cast<CpmObj&>(ptrCpmObjectIt->second);
        } else {
            throw ("no object found for uid : "+to_string(uid));
        }
    }
    
};

class CpmManager : public StorageCpmObject {
    
    public :
    friend class MyTestSuite;
    
    CpmManager(string path, string name, uint32_t firstUID = 100);
    CpmManager(CpmManager &copy, uint32_t firstUID = 100);
    
    virtual string getPath();
    uint32_t getNextUID();
    
    void removeConversationHistory(uint32_t uid);
    void removeConversationHistory(string name);
    
    void read();
    void write();
    
    static const string objectType;
    
    private :
    
    uint32_t nextUID;
    
    string defaultPath;
    static mutex mMutex;
    
    virtual const string& getObjectType();
    
    static const vector<string> knownCPMtypeObjects;
    virtual const vector<string>& getKnownCPMtypeObjects();
    
    static const shared_ptr<StorageCpmObject> cpmSeed;
};

#endif
