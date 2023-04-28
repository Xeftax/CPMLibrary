#ifndef CPMMANAGER_H
#define CPMMANAGER_H

#include <cstdint>
#include <string>
#include <list>
#include <map>
#include <mutex>
#include "storagecpmobject.h"
#include <log4cxx/logger.h>
using namespace log4cxx;
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
        
        CpmManager(string path, string name);
        CpmManager(CpmManager &copy);

        virtual const uint32_t getUID();
        virtual const string getFolderName();
        virtual const uint32_t getUIDValidity();

        virtual string getPath();
        uint32_t getNextUID();

        static string UIDstoHexString(pair<uint32_t,uint32_t> uids);
        static pair<uint32_t,uint32_t> hexStringtoIntUIDs(string hexuid);
        static string folderPathCheck(string path);
        
        static shared_ptr<CpmObject> read(string path);
        static string write(shared_ptr<CpmObject> cpmObject);
        
        static const string objectType;
    
    private :
        
        string defaultPath;
        static mutex mMutex;
        
        virtual const string& getObjectType();
        
        static const vector<string> knownCpmObjectTypes;
        static const vector<string> allowedCpmObjectTypes;
        virtual const vector<string>& getAllowedCpmObjectTypes();
        
        static Logger logger;
};

#endif
