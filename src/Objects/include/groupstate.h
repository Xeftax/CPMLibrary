#ifndef GROUPSTATE_H
#define GROUPSTATE_H

#include <list>

#include "cpmobject.h"

using namespace std;

struct dateComparator {
    bool operator()(const string &a, const string &b) const;
};

struct groupStateComparator {
    bool operator()(const GroupStateHeaders &a, const GroupStateHeaders &b) const;
};

class GroupState : public CpmObject, public enable_shared_from_this<GroupState> {
    
    public :
    friend class SessionHistory;
    
    GroupState();
    GroupState(string body);
    GroupState(GroupState &copy);
    
    GroupStateHeaders headers;
    GroupeStateParticipants participants;
    
    map<GroupStateHeaders,GroupeStateParticipants,groupStateComparator> getHistory();
    void update();
    
    virtual string preview();
    
    virtual bool checkWritingIntegrity();
    virtual bool isComplete();
    
    static const string objectType;
        
    private :
    
    map<GroupStateHeaders,GroupeStateParticipants,groupStateComparator> groupStateHistory;
    
    void setGroupState(string content);
    static map<string,string> reader(string content);
    
    virtual const string& getObjectType();
    

};

#endif
