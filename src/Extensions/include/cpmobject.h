#ifndef CPMOBJECT_H
#define CPMOBJECT_H

#include <cstdint>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <list>
#include <map>
#include "cpmheaders.h"
#include "xcpm.h"

using namespace std;

class MyTestSuite;
class StorageCpmObject;

class CpmObject {
    
    public :
    friend MyTestSuite;
    friend StorageCpmObject;
    
    CpmObject();
    CpmObject(CpmObject &copy);
        
    const uint32_t getUID();
    virtual const string& getFolderName();
    virtual const uint32_t getUIDValidity();
    
    virtual void write(string path, bool verif = true);
    virtual void read(string path);
    
    static const string objectType;
        
    protected :

    uint32_t UID = 0;
    weak_ptr<StorageCpmObject> mParent;
    
    string typeRead;
    headersMap headersRead;
    string bodyRead;
    void cleanCache();
    
    string UIDstoHexString();
    pair<uint32_t,uint32_t> hexStringtoIntUIDs(string hexuid);
    
    const static string mUnknownString;
    virtual const string& getObjectType();
    string folderPathCheck(string path);
    virtual bool cpmObjectValidity();
    virtual bool isEqual(shared_ptr<CpmObject> cpmObject);
    
    bool writeVerif(shared_ptr<CpmObject> newCpmObject, string path);
    
};

#endif
