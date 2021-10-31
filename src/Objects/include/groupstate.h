#ifndef GROUPSTATE_H
#define GROUPSTATE_H

#include <list>
#include <string>

#include "cpmobject.h"

using namespace std;

class GSParticipant {
    
public :
    
    string name;
    string commADDR;
    pair<string,string> get();
    
private :
    
    pair<string,string> participant;
    
};

class GroupStateElement {
    
public :
    
    string LASTFOCUSSESSIONID;
    string TIMESTAMP;
    string GROUP_TYPE;
    
};

class Participants {
    
public :
    
    void addParticipant(string name, string comm_addr);
    void addParticipant(GSParticipant participant);
    void addParticipants(Participants participants);
            
    list <GSParticipant> getParticipants(){
        return participantList;
    }
    
private :
    
    list <GSParticipant> participantList;

};

class GroupState : CpmObject {
        
    GroupStateElement groupStateElement;
    Participants participants;
    string makeXML();

};

#endif
