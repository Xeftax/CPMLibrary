#ifndef STORAGECPMOBJECT_H
#define STORAGECPMOBJECT_H

#include <set>
#include "cpmobject.h"
#include "errors.h"

#define PATH_SEP "/"

typedef multimap<string, shared_ptr<CpmObject>>::iterator ItCpmMap;

template<typename CpmObj> class MapByType;

class StorageCpmObject : public CpmObject, public enable_shared_from_this<StorageCpmObject> {
    public :
    friend MyTestSuite;
    
    StorageCpmObject(string name);
    StorageCpmObject(StorageCpmObject &copy);
    
    string& getName();
    virtual string getPath();
    virtual uint32_t getNextUID();
    
    virtual void append(shared_ptr<CpmObject> cpmObject);
    
    template<typename CpmObj, typename... Args>
    shared_ptr<CpmObj> createNew(Args... args);
    
    template<typename CpmObj = CpmObject>
    MapByType<CpmObj> fetch();
    
    virtual void write(string path, bool verif = true);
    
    virtual void read(string path);
    
    static const string objectType;
    
    protected :

    string NAME;
    
    multimap<string, shared_ptr<CpmObject>> cpmObjectsList;
    set<uint32_t> givenUIDs;
    
    virtual const string& getObjectType();
    
    static const vector<string> knownCPMtypeObjects;
    virtual const vector<string>& getKnownCPMtypeObjects();
    
    virtual bool isEqual(shared_ptr<CpmObject> cpmObject);

};



#endif  
