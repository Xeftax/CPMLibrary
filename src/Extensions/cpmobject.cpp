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

CpmObject::CpmObject() {}
CpmObject::CpmObject(CpmObject &copy) : headersRead(copy.headersRead), bodyRead(copy.bodyRead) {
    cout << "warning : CPM Object copied." << endl;
}

const uint32_t CpmObject::getUID() { return UID;}
const string& CpmObject::getFolderName() {
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return (parent?parent->getName():mUnknownString);
}
const uint32_t CpmObject::getUIDValidity() {
    shared_ptr<StorageCpmObject> parent = mParent.lock();
    return (parent?parent->getUID():0);
}

string CpmObject::UIDstoHexString(){
    ostringstream stream;
    stream << setfill ('0') << setw(sizeof(uint32_t)*2) << hex << getUIDValidity() << setfill ('0') << setw(sizeof(uint32_t)*2) << hex << getUID();
    return stream.str();
}

pair<uint32_t,uint32_t> CpmObject::hexStringtoIntUIDs(string hexuid){
    string uidValStr = hexuid.substr(0,8);
    string uidStr = hexuid.substr(8,16);
    uint32_t uidValidity = stoul(uidValStr, 0, 16);
    uint32_t uid = stoul(uidStr, 0, 16);
    return pair<uint32_t,uint32_t>(uidValidity,uid);
}

bool CpmObject::cpmObjectValidity(){
    if (not mParent.lock()) throw Errors(UnlinkedCpmObject);
    return true;
}

void CpmObject::write(string path, bool verif){
    const string except = "error while writing "+path;
    path = folderPathCheck(path);
    try {
        cpmObjectValidity();
    } catch (Errors const &e) {
        throw Errors(e,except);
    }
    
    if (verif) {
        int _pos = path.find_last_of("_");
        string r_name = path.substr(_pos -17);
        string t_name = PATH_SEP+mParent.lock()->UIDstoHexString()+"_"+getFolderName()+PATH_SEP;
        if (r_name != t_name) throw Errors(InconsistentPath, except+" : the path does not match with the parent Storage CPM Object path");
    }
    
}

void CpmObject::read(string path){
    const string except = "error while reading "+path;
    ifstream newFile(path);
    if (not newFile.is_open()){
        throw Errors(WrongPath, except+" : unable to open the file");
    }
    
    string line;
    int lineIndex = 0;
    int fileSection = 0;
    ostringstream body;
    while (getline(newFile,line)){
        if (line == "") {
            ++fileSection;
        } else {
            switch (fileSection) {
                case 0: {
                    int firstHeaderSize = sizeof(HEADER_CONTENT_TYPE HEADER_SEP)/sizeof(char)-1;
                    if (lineIndex >= 0 and line.substr(0,firstHeaderSize) != HEADER_CONTENT_TYPE HEADER_SEP) {
                        throw Errors(IncompatibleCpmObject, except+" : 1st line is wrong (bad syntax)");
                    }
                    typeRead = line.substr(firstHeaderSize);
                    break;
                }
                    
                case 1: {
                    size_t headerSep = line.find(HEADER_SEP);
                    if (headerSep != string::npos) {
                        string header = line.substr(0,headerSep);
                        string content = line.substr(headerSep +sizeof(HEADER_SEP)/sizeof(char) -1);
                        headersRead.insert(make_pair(header,content));
                    } else {
                        cout << "warning : invalid header, line " << line << "in" << path << endl;
                    }
                    break;
                }
                    
                case 2:
                    body << line;
                    ++fileSection;
                    break;
                    
                default:
                    body << endl << line;
                    break;
            }
        }
        lineIndex++;
    }
    bodyRead = body.str();
    newFile.close();
    
    string name = path.substr(path.find_last_of("/")+1);
    UID = hexStringtoIntUIDs(name).second;
    if (typeRead != getObjectType()) throw Errors(IncompatibleCpmObject,except+" : 1st line is wrong (Content Type does not match)");
    if (fileSection < 2) throw Errors(IncompatibleCpmObject, except+" : the layout of the file does not match with the layout of a CPM Object file");
}

void CpmObject::cleanCache() {
    headersRead.clear();
    bodyRead.clear();
    typeRead.clear();
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

bool CpmObject::isEqual(shared_ptr<CpmObject> cpmObject) {
    shared_ptr<CpmObject> p1 = mParent.lock();
    shared_ptr<CpmObject> p2 = cpmObject->mParent.lock();
    if (UID != cpmObject->UID or getUIDValidity() != cpmObject->getUIDValidity() or getFolderName() != cpmObject->getFolderName())
        throw Errors(IDsUnequality);
    if (headersRead != cpmObject->headersRead or bodyRead != cpmObject->bodyRead)
        throw Errors(CacheUnequality);
    if (getObjectType() != cpmObject->getObjectType())
        throw Errors(TypeUnequality);
    return true;
}

bool CpmObject::writeVerif(shared_ptr<CpmObject> newCpmObject, string path) {
    newCpmObject->mParent = this->mParent;
    newCpmObject->read(path);
    return isEqual(newCpmObject);
}
