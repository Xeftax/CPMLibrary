#ifndef SESSIONHISTORY_H
#define SESSIONHISTORY_H

#include "conversationhistory.h"
#include "sessioninfo.h"
#include "groupstate.h"

class SessionHistory : public StorageCpmObject {
    
    public :
    
    SessionHistory();
    SessionHistory(SessionHistory &copy);
    
    static const string objectType;
    
    shared_ptr<SessionInfo> sessionInfo = make_shared<SessionInfo>();
    void setSessionInfo(SessionInfo &sessionInfo);
    
    shared_ptr<GroupState> groupState = make_shared<GroupState>();
    void setGroupState(GroupState &groupState);
    
    virtual bool checkWritingIntegrity();
    virtual bool isComplete();
    
    private :
    
    virtual const string& getObjectType();
    
    static const vector<string> allowedCpmObjectTypes;
    virtual const vector<string>& getAllowedCpmObjectTypes();
    
    

};

#endif
