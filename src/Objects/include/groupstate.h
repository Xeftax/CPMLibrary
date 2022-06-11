#ifndef GROUPSTATE_H
#define GROUPSTATE_H

#include <list>

#include "cpmobject.h"

using namespace std;

class GroupState : public CpmObject {
    
    public :
    GroupState();
    GroupState(map<string,string> headers, string body);
    GroupState(GroupState &copy);
    
    GroupeStateHeaders headers;
    GroupeStateParticipants participants;
    
    static const string objectType;
        
    private :

    string format();
    
    virtual const string& getObjectType();
    

};

#endif
