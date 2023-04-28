#ifndef CONVERSATIONISTORY_H
#define CONVERSATIONISTORY_H

#include <map>
#include <list>
#include "storagecpmobject.h"

class CpmManager;
class SessionHistory;

class ConversationHistory : public StorageCpmObject {
    
    public :
    friend CpmManager;
    
    ConversationHistory(string name);
    ConversationHistory(ConversationHistory &copy);
    
    static const string objectType;
    
    private :
    
    virtual const string& getObjectType();
    static const vector<string> allowedCpmObjectTypes;
    virtual const vector<string>& getAllowedCpmObjectTypes();
    
};

#endif
