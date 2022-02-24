#include "storagecpmobject.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"
#include "cpmmanager.h"
#include "errors.h"

StorageCpmObject::StorageCpmObject(string name) : NAME(name) {}
StorageCpmObject::StorageCpmObject(StorageCpmObject &copy) : CpmObject(copy), NAME(copy.NAME), cpmObjectsList(copy.cpmObjectsList) {}

string& StorageCpmObject::getName(){
    return NAME;
}

string StorageCpmObject::getPath(){
    const string except = "error while getting the Storage Cpm Object path";
    try {
        cpmObjectValidity();
    } catch(Errors const &e) {
        throw Errors(e, except);
    }
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return parent->getPath() + parent->NAME + PATH_SEP;
}

uint32_t StorageCpmObject::getNextUID(){
    const string except = "error while giving an UID";
    try {
        cpmObjectValidity();
    } catch(Errors const &e) {
        throw Errors(e, except);
    }
    return mParent.lock()->getNextUID();
}

void StorageCpmObject::append(shared_ptr<CpmObject> cpmObject) {
    const string except = "error while adding a '"+(cpmObject?cpmObject->getObjectType():"nullptr")+"' type in a '"+getObjectType();
    try {
        cpmObjectValidity();
    } catch (Errors const &e) {
        throw Errors(e,except);
    }
    if (cpmObject == nullptr) throw Errors(-1,except);
    if (cpmObject->mParent.lock()) throw Errors(LinkedCpmObject, except);
    const vector<string> &knownObjects = getKnownCPMtypeObjects();
    if (find(knownObjects.begin(), knownObjects.end(), cpmObject->getObjectType()) == knownObjects.end()) {
        throw Errors(IncompatibleCpmObject, except);
    }
    
    try {
        cpmObject->mParent = shared_from_this();
    } catch(exception const &e) {
        throw Errors(-1,except+" : "+e.what()+", Storage CPM Objects must be shared_ptr");
    }
    if (cpmObject->UID == 0) cpmObject->UID = getNextUID();
    if (not givenUIDs.insert(cpmObject->UID).second) throw Errors(InvalidUID, except);
    cpmObjectsList.insert(make_pair(cpmObject->getObjectType(), cpmObject));
}

template<typename CpmObj>
MapByType<CpmObj> StorageCpmObject::fetch(){
    MapByType<CpmObj> mapByType;
    
    pair<ItCpmMap,ItCpmMap> itByType(cpmObjectsList.begin(),cpmObjectsList.end());
    if (CpmObj::objectType != CpmObject::objectType){itByType = cpmObjectsList.equal_range(CpmObj::objectType);}
    for(ItCpmMap it = itByType.first;  it != itByType.second;  ++it){
        uint32_t uid = it->second->getUID();
        shared_ptr<CpmObject> ptrCpmObject = it->second;
        mapByType.insert(make_pair(uid, ptrCpmObject));
    }
    return mapByType;
}

template MapByType<CpmObject> StorageCpmObject::fetch<CpmObject>();
template MapByType<ConversationHistory> StorageCpmObject::fetch<ConversationHistory>();
template MapByType<SessionHistory> StorageCpmObject::fetch<SessionHistory>();
template MapByType<Message> StorageCpmObject::fetch<Message>();
template MapByType<FileTransferHistory> StorageCpmObject::fetch<FileTransferHistory>();
template MapByType<Media> StorageCpmObject::fetch<Media>();
template MapByType<GroupState> StorageCpmObject::fetch<GroupState>();

template<typename CpmObj, typename... Args>
shared_ptr<CpmObj> StorageCpmObject::createNew(Args... args) {
    shared_ptr<CpmObj> sharedCpmObject = make_shared<CpmObj>(args...);
    append(sharedCpmObject);
    return sharedCpmObject;
}

template shared_ptr<ConversationHistory> StorageCpmObject::createNew<ConversationHistory>(string);
template shared_ptr<ConversationHistory> StorageCpmObject::createNew<ConversationHistory>(char const *);
template shared_ptr<SessionHistory> StorageCpmObject::createNew<SessionHistory>();
template shared_ptr<Message> StorageCpmObject::createNew<Message>();
template shared_ptr<FileTransferHistory> StorageCpmObject::createNew<FileTransferHistory>();
template shared_ptr<Media> StorageCpmObject::createNew<Media>();
template shared_ptr<GroupState> StorageCpmObject::createNew<GroupState>();


void StorageCpmObject::write(string path, bool verif){
    cpmObjectValidity();
    path = folderPathCheck(path);
    string completePath;
    getUIDValidity()?completePath = path+UIDstoHexString()+"_"+NAME:completePath = path+NAME;
    filesystem::create_directory(completePath);
    MapByType<CpmObject> allCpmObjects = fetch();
    for (auto& pair : allCpmObjects) {
        pair.second->write(completePath, false);
    }
    shared_ptr<CpmObject> compareCpmObject;
    ///
    ///
    if (getObjectType() == SessionHistory::objectType) compareCpmObject = make_shared<SessionHistory>();
    else if (getObjectType() == ConversationHistory::objectType)
        compareCpmObject = make_shared<ConversationHistory>(NAME);
    else if (getObjectType() == CpmManager::objectType) compareCpmObject = make_shared<CpmManager>(path, NAME, UID);
    else verif = false;
    ///
    ///
    if (verif) writeVerif(compareCpmObject, completePath);
    
}

void StorageCpmObject::read(string path){
    const string except = "error while reading "+path+" : unable to recognize the file type";
    for (const auto &entry : filesystem::directory_iterator(path)) {
        string path = entry.path();
        string name = path.substr(path.find_last_of("/")+1);
        string type;
        
        shared_ptr<CpmObject> newCpmObject;
        pair<uint32_t, uint32_t> uids;
        try {
            uids = hexStringtoIntUIDs(name);
        } catch(exception const &e) {
            throw Errors(IncompatibleCpmObject, except);
        }
        
        string parentName = path.substr(0,path.find_last_of("/"));
        parentName = parentName.substr(parentName.find_last_of("/")+1);
        if (name.find("_") != string::npos) {
            if (parentName.find("_") != string::npos) newCpmObject = make_shared<SessionHistory>();
            else newCpmObject = make_shared<ConversationHistory>(name.substr(name.find("_")));
        } else {
            shared_ptr<CpmObject> cpmObject = make_shared<CpmObject>();
            try {
                cpmObject->read(path);
            } catch(Errors const &e) {
                if (e.getCode() != IncompatibleCpmObject or typeRead.empty()) throw e;
            }
            string type = cpmObject->typeRead;
            
            ///
            ///
            if (type == Message::objectType) newCpmObject = make_shared<Message>();
            else if (type == FileTransferHistory::objectType) newCpmObject = make_shared<FileTransferHistory>();
            else if (type == SessionInfo::objectType) newCpmObject = make_shared<SessionInfo>();
            else if (type == GroupState::objectType) newCpmObject = make_shared<GroupState>();
            else if (type == Media::objectType) newCpmObject = make_shared<Media>();
            else newCpmObject = make_shared<CpmObject>();
            ///
            ///
            
            newCpmObject->typeRead = cpmObject->typeRead;
            newCpmObject->headersRead = cpmObject->headersRead;
            newCpmObject->bodyRead = cpmObject->bodyRead;
        }
        
        newCpmObject->UID = uids.second;
        newCpmObject->read(path);
        append(newCpmObject);
    }
}

const string& StorageCpmObject::getObjectType() {
    return objectType;
}

const vector<string>& StorageCpmObject::getKnownCPMtypeObjects() {
    return knownCPMtypeObjects;
}

bool StorageCpmObject::isEqual(shared_ptr<CpmObject> cpmObject) {
    CpmObject::isEqual(cpmObject);
    shared_ptr<StorageCpmObject> storageCpmObject = dynamic_pointer_cast<StorageCpmObject>(cpmObject);
    if (NAME != storageCpmObject->NAME)
        throw Errors(IDsUnequality);
    if (cpmObjectsList.size() != storageCpmObject->cpmObjectsList.size())
        throw Errors(ContentUnequality);
    pair<ItCpmMap,ItCpmMap> its(cpmObjectsList.begin(),cpmObjectsList.end());
    ItCpmMap it2 = storageCpmObject->cpmObjectsList.begin();
    for(ItCpmMap it1 = its.first;  it1 != its.second;  ++it1){
        if (it1->first != it2->first or it1->second->getUID() != it2->second->getUID())
            throw Errors(ContentUnequality);
        it1->second->isEqual(it1->second);
        ++it2;
    }
    return true;
}


