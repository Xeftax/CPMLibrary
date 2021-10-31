#include "groupstate.h"


string GroupState::makeXML(){
    string xml = "<groupstate timestamp=\"" + groupStateElement.TIMESTAMP  // ex : ”2012-06-13T16:39:57-05:00”
        + "\" lastfocussessionid=\"" + groupStateElement.LASTFOCUSSESSIONID // ex : ”da9274453@company.com”
        + "\" group-type=\"" + groupStateElement.GROUP_TYPE // ex : "closed"
        + "\">";
    
    for (auto& p : participants.getParticipants()){
        xml += "<participant name=\"" + p.get().first + "\" comm-addr=\"" + p.get().second + "\"/>";
    }
    
    xml += "</groupstate>";

    return xml;
}


void Participants::addParticipant(string name, string comm_addr){
    GSParticipant participant;
    participant.name = name;
    participant.commADDR = comm_addr;
    participantList.push_back(participant);
}

void Participants::addParticipant(GSParticipant participant){
    participantList.push_back(participant);
}

void Participants::addParticipants(Participants participants){
    participantList.splice(participantList.end(), participants.participantList);
}

pair<string,string> GSParticipant::get(){
    return participant;
}
