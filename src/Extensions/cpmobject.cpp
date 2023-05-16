#include "cpmobject.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"
#include "cpmmanager.h"
#include "errors.h"
#include <sys/stat.h>
#include <unistd.h>

const string CpmObject::mUnknownString = "Unknown";

//const string CpmObject::nullParent = StorageCpmObject("nullParent");

CpmObject::CpmObject() {}
CpmObject::CpmObject(CpmObject &copy) {}
CpmObject::~CpmObject() {}

const uint32_t CpmObject::getUID() { 
    if (not mParent.lock()) throw Errors(UnlinkedCpmObject, "unable to get the UID of this CPM Object because it does not have one");
    return UID;
}

const string CpmObject::getFolderName() {
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    if (not parent) throw Errors(UnlinkedCpmObject, "unable to get the folder name of this CPM Object because it does not have one");
    return parent->getName();
}
const uint32_t CpmObject::getUIDValidity() {
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    if (not parent) throw Errors(UnlinkedCpmObject, "unable to get the UID validity of this CPM Object because it does not have one");
    return (parent?parent->getUID():0);
}

string CpmObject::getPath(){
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return parent?parent->getPath()+PATH_SEP+UIDstoHexString():throw Errors(UnlinkedCpmObject, "unable to get the path of this CPM Object because it does not have one");
}


string CpmObject::UIDstoHexString(){
    return CpmManager::UIDstoHexString(make_pair(getUIDValidity(), getUID()));
}

void CpmObject::hexStringtoIntUIDs(string hexuid){
    pair<uint32_t,uint32_t> uids = CpmManager::hexStringtoIntUIDs(hexuid);
    UID = uids.second;
    if (getUIDValidity() != uids.first) throw Errors(InconsistentPath,"parent UID does not match with the file name");
}


string CpmObject::preview() {
    ostringstream content;
    content << HEADER_CONTENT_TYPE << HEADER_SEP << getObjectType() << endl;
    return content.str();
}

const string& CpmObject::getObjectType() {
    return objectType;
}

bool CpmObject::checkWritingIntegrity() {
    filesystem::path path = getPath();
    string name = path.filename();
    string folderName = path.parent_path().filename();
    if (folderName.find_first_not_of("0123456789abcdef_") >= 17) folderName.erase(0,17);
    pair<uint32_t,uint32_t> uids = CpmManager::hexStringtoIntUIDs(name.substr(0,16));
    if (UID != uids.second or getUIDValidity() != uids.first or getFolderName() != folderName)
        throw Errors(IDsUnequality);
    return true;
}

bool CpmObject::isComplete(){
    return true;
}
