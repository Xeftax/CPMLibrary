#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include "cpmobject.h"
#include "media.h"

class SessionHistory;

class SessionInfo : public CpmObject {
    
    public :
    friend SessionHistory;
    
    SessionInfo();
    SessionInfo(map<string,string> headers_map, string body);
    SessionInfo(SessionInfo &copy);
            
    SessionInfoHeaders headers;
    XCPM xcpm{"Application/X-CPM-Session" ,"session-type"};
    
    virtual string preview();
    
    static const string objectType;
    virtual const string& getContentType();
    
    virtual bool checkWritingIntegrity();
    virtual bool isComplete();
    
    private :
    static string mContentType;
    virtual const string& getObjectType();

};

#endif
