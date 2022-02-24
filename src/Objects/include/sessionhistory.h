#ifndef SESSIONHISTORY_H
#define SESSIONHISTORY_H

#include "conversationhistory.h"
#include "sessioninfo.h"

class SessionHistory : public StorageCpmObject {
    
    public :
    
    SessionHistory();
    SessionHistory(SessionHistory &copy);
    
    static const string objectType;
    
    shared_ptr<SessionInfo> sessionInfo = make_shared<SessionInfo>();
    void setSessionInfo(SessionInfo &sessionInfo);
    
    private :
    
    virtual const string& getObjectType();
    
    static const vector<string> knownCPMtypeObjects;
    virtual const vector<string>& getKnownCPMtypeObjects();
    
    

};

#endif
