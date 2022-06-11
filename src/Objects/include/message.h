#ifndef MESSAGE_H
#define MESSAGE_H

#include "cpmobject.h"

class Message : public CpmObject {
    
    public :
    friend class MyTestSuite;
    
    Message();
    Message(map<string,string> headers, string body);
    Message(Message &copy);
    
    MessageHeaders headers;
    string message_body;

    virtual string preview();
    
    static const string objectType;
    
    virtual bool checkWritingIntegrity();
    virtual bool isComplete();
        
    private :
    
    virtual const string& getObjectType();
    
};

#endif
