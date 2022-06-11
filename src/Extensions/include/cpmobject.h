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
#include <log4cxx/logger.h>
#include "cpmheaders.h"
#include "xcpm.h"

using namespace std;

class MyTestSuite;
class StorageCpmObject;
class CpmManager;

class CpmObject {
    
    public :
    friend MyTestSuite;
    friend StorageCpmObject;
    friend CpmManager;
    
    CpmObject();
    CpmObject(CpmObject &copy);
    virtual ~CpmObject();
        
    virtual const uint32_t getUID();
    virtual const string getFolderName();
    virtual const uint32_t getUIDValidity();
    virtual string getPath();
    
    virtual string preview();
    
    static const string objectType;
    
    virtual bool checkWritingIntegrity();
    virtual bool isComplete();
        
    protected :

    uint32_t UID = 0;
    weak_ptr<StorageCpmObject> mParent;
    void setParent(shared_ptr<StorageCpmObject>);
    
    string UIDstoHexString();
    void hexStringtoIntUIDs(string hexuid);
    
    const static string mUnknownString;
    //const static StorageCpmObject nullParent;
    virtual const string& getObjectType();
    string folderPathCheck(string path);
    
};

#endif
