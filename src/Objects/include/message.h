#ifndef MESSAGE_H
#define MESSAGE_H

#include "cpmobject.h"

class Message : public CpmObject {
    
    public :
    friend class MyTestSuite;
    
    Message();
    Message(Message &copy);
    
    virtual void write(string path, bool verif = true);
    virtual void read(string path);
    MessageHeaders headers;
    string message_body;

    
    static const string objectType;
        
    private :
    
    virtual const string& getObjectType();
    virtual bool isEqual(shared_ptr<CpmObject> cpmObject);
    virtual bool cpmObjectValidity();
    
};

#endif
