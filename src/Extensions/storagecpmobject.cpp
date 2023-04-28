#include <type_traits>
#include "storagecpmobject.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"
#include "cpmmanager.h"
#include "errors.h"

//LoggerPtr logger(Logger::getLogger( "main"));


StorageCpmObject::StorageCpmObject(string name) : NAME(name) {
    const string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890()_- ";
    for (const char& c : name) {
        if (allowedChars.find(c) == string::npos)
            throw Errors(NameError,"storage cpm object name can only contains this characters : \""+allowedChars+"\" (found: \""+name+"\")");
    }
}
StorageCpmObject::StorageCpmObject(StorageCpmObject &copy) : CpmObject(copy), NAME(copy.NAME), storageCpmObjectList(copy.storageCpmObjectList), cpmObjectUIDList(copy.cpmObjectUIDList) {}

string& StorageCpmObject::getName(){
    return NAME;
}

string StorageCpmObject::getPath(){
    return CpmObject::getPath()+"_"+NAME;
}

uint32_t StorageCpmObject::getNextUID(){
    return mParent.lock()?mParent.lock()->getNextUID():throw Errors(UnlinkedCpmObject, "error while giving an UID");
}

uint32_t StorageCpmObject::append(shared_ptr<CpmObject> cpmObject) {
    const string except = "error while adding a '"+cpmObject->getObjectType()+"' type in a '"+getObjectType();
    if (cpmObject->mParent.lock()) throw Errors(LinkedCpmObject, except);
    const vector<string> &allowedObjects = getAllowedCpmObjectTypes();
    if (find(allowedObjects.begin(), allowedObjects.end(), cpmObject->getObjectType()) == allowedObjects.end())
        throw Errors(IncompatibleCpmObject, except);
    
    
    try {
        cpmObject->mParent = shared_from_this();
    } catch(exception const &e) {
        throw Errors(-1,except+" : "+e.what()+", Storage CPM Objects must be shared_ptr");
    }
    cpmObject->UID = getNextUID();
    CpmManager::write(cpmObject);
    
    if (dynamic_pointer_cast<StorageCpmObject>(cpmObject))
        storageCpmObjectList.insert(make_pair(cpmObject->getUID(), cpmObject));
    else
        cpmObjectUIDList.insert(cpmObject->getUID());
    return cpmObject->UID;
}

void StorageCpmObject::scan() {
    shared_ptr<StorageCpmObject> parent = dynamic_pointer_cast<SessionHistory>(shared_from_this());
    for (const auto &entry : filesystem::directory_iterator(getPath())) {
        string path = entry.path();
        string name = path.substr(path.rfind(PATH_SEP) +1,16);
        try {
            pair<uint32_t,uint32_t> uids = CpmManager::hexStringtoIntUIDs(name);
            if (storageCpmObjectList.find(uids.second) != storageCpmObjectList.end() or cpmObjectUIDList.find(uids.second) != cpmObjectUIDList.end()) continue;
            if (getUID() != uids.first) throw Errors(UIDValidityError,"inconsistency detected with the uid and the parent directory of the Cpm Object at "+path);
            size_t _pos = name.find("_");
            if (_pos != string::npos) {
                shared_ptr<StorageCpmObject> storageCpmObject = dynamic_pointer_cast<StorageCpmObject>(CpmManager::read(path));
                storageCpmObject->mParent = shared_from_this();
                storageCpmObject->hexStringtoIntUIDs(name.substr(0, _pos));
                storageCpmObjectList.insert(make_pair(storageCpmObject->getUID(), storageCpmObject));
                storageCpmObject->scan();
            } else {
                if (not parent or uids.second != UID+1)
                    cpmObjectUIDList.insert(uids.second);
            }
        } catch(exception const &e) {
            cout << e.what() << endl;
            //LOG4CXX_ERROR(logger, "unable to treat the file : " << path << " : " << e.what());
        } catch(string const &str) {
            cout << str << endl;
        }
    }
}

shared_ptr<CpmObject> StorageCpmObject::fetchByUID(uint32_t uid) {
    auto it1 = cpmObjectUIDList.find(uid);
    if (it1 != cpmObjectUIDList.end()) {
        string fileName = CpmManager::UIDstoHexString(make_pair(getUID(), uid));
        shared_ptr<CpmObject> cpmObject;
        cpmObject->UID = uid;
        cpmObject->mParent = shared_from_this();
        cpmObject = CpmManager::read(getPath()+fileName);
        return cpmObject;
    }
    auto it2 = storageCpmObjectList.find(uid);
    if (it2 != storageCpmObjectList.end()) return it2->second;

    throw Errors(InvalidUID,"error in fetching the object with the uid : "+to_string(uid));
}

shared_ptr<CpmObject> StorageCpmObject::fetch(int increment) {
    uint32_t uid = *fetchIterator;
    advance(fetchIterator, increment);
    return fetchByUID(uid);
}

const string& StorageCpmObject::getObjectType() {
    return objectType;
}

const vector<string>& StorageCpmObject::getAllowedCpmObjectTypes() {
    return allowedCpmObjectTypes;
}

bool StorageCpmObject::checkWritingIntegrity() {
    shared_ptr<StorageCpmObject> storageCpmObject = dynamic_pointer_cast<StorageCpmObject>(CpmManager::read(getPath()));
    if (cpmObjectUIDList != storageCpmObject->cpmObjectUIDList or storageCpmObjectList != storageCpmObject->storageCpmObjectList)
        throw Errors(ContentUnequality);
    if (NAME != storageCpmObject->NAME)
        throw Errors(IDsUnequality);
    return CpmObject::checkWritingIntegrity();
}


