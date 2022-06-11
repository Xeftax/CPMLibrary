#include "cpmobject.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"
#include "cpmmanager.h"
#include "errors.h"

const string CpmObject::mUnknownString = "Unknown";

//const string CpmObject::nullParent = StorageCpmObject("nullParent");

CpmObject::CpmObject() {}
CpmObject::CpmObject(CpmObject &copy) {}
CpmObject::~CpmObject() {}

const uint32_t CpmObject::getUID() { return UID;}

const string CpmObject::getFolderName() {
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return (parent?parent->getName():mUnknownString);
}
const uint32_t CpmObject::getUIDValidity() {
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return (parent?parent->getUID():0);
}

string CpmObject::getPath(){
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return parent?parent->getPath()+UIDstoHexString():throw Errors(UnlinkedCpmObject, "error while asking the Cpm Object path");
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

string CpmObject::folderPathCheck(string path){
    ifstream pathCheck(path);
    if (not pathCheck)
        throw Errors(WrongPath, "error in trying to read-write files at this path : " + path);
    if (path.substr(path.length()-1) != PATH_SEP) {
        path = path + PATH_SEP;
    }
    return path;
}

bool CpmObject::checkWritingIntegrity() {
    string path = getPath();
    int sepSize = sizeof(PATH_SEP)/sizeof(char) -1;
    if (path.rfind(PATH_SEP) == path.length()-sepSize) path.pop_back();
    size_t sep_pos1 = path.rfind(PATH_SEP);
    size_t sep_pos2 = path.rfind(PATH_SEP, sep_pos1 -1) + sepSize;
    string name = path.substr(sep_pos1 + sepSize);
    string folderName = path.substr(sep_pos2, sep_pos1-sep_pos2);
    if (folderName.find_first_not_of("0123456789abcdef_") >= 17) folderName.erase(0,17);
    pair<uint32_t,uint32_t> uids = CpmManager::hexStringtoIntUIDs(name);
    if (UID != uids.second or getUIDValidity() != uids.first or getFolderName() != folderName)
        throw Errors(IDsUnequality);
    return true;
}

bool CpmObject::isComplete(){
    return true;
}
