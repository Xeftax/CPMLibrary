#ifndef STORAGECPMOBJECT_H
#define STORAGECPMOBJECT_H

#include <set>
#include "cpmobject.h"
#include "errors.h"

#define PATH_SEP "/"

template<typename CpmObj> class MapByType;

class StorageCpmObject : public CpmObject, public enable_shared_from_this<StorageCpmObject> {
    public :
    friend MyTestSuite;
    
    StorageCpmObject(string name);
    StorageCpmObject(StorageCpmObject &copy);
    
    string& getName();
    virtual string getPath();
    virtual uint32_t getNextUID();
    
    virtual void scan();
    virtual uint32_t append(shared_ptr<CpmObject> cpmObject);
    virtual shared_ptr<CpmObject> fetchByUID(uint32_t uid);
    virtual shared_ptr<CpmObject> fetch(int increment = 1);
    
    template<typename CpmObj, typename... Args>
    shared_ptr<CpmObj> createNew(Args... args);
    
    static const string objectType;
    
    virtual bool checkWritingIntegrity();
    
    protected :

    string NAME;
    map<uint32_t,shared_ptr<CpmObject>> storageCpmObjectList;
    set<uint32_t, greater<uint32_t>> cpmObjectUIDList;
    set<uint32_t, greater<uint32_t>>::iterator fetchIterator;
    
    virtual const string& getObjectType();
    
    static const vector<string> allowedCpmObjectTypes;
    virtual const vector<string>& getAllowedCpmObjectTypes();
    


    //static Logger logger;
};



#endif  
