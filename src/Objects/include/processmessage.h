#ifndef PROCESSMESSAGE_H
#define PROCESSMESSAGE_H

#include <list>
#include <iostream>

#include "message.h"

using namespace std;

class Headers {
        
public :
    
    Headers(string content_type) : CONTENT_TYPE(content_type) {};
        
    string FROM;
    string DATE;
    string subject = "";
    string CONVERSATION_ID;
    string CONTRIBUTION_ID;
    string in_reply_to_contribution_id = "";

    string getCONTENT_TYPE();
    
    virtual stringMap getMap();
    
    virtual string getText();
        
protected:
    
    bool mMessageBody = false;
    string CONTENT_TYPE;
        
};

class XCPM {
public :
    int TYPE;
    list <string> INVITED_PARTICIPANTS;
    string CONTENT_ID;
    string SDP_PARAMETERS;
};

#endif
