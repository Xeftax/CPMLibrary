#ifndef GROUPSTATE_H
#define GROUPSTATE_H

#include <list>
#include <string>

#include "cpmobject.h"

using namespace std;

class GroupState : CpmObject {

    private :

        class GroupStateElement {
            public :
                string LASTFOCUSSESSIONID;
                string TIMESTAMP;
                string GROUP_TYPE;       
        };

        class Participants {
            private :
                list <pair<string,string>> participants;
            public :
                void addParticipant(string name, string comm_addr){
                    pair <string,string> participant = {name, comm_addr};
                    participants.push_back(participant);
                }
                list <pair<string,string>> getParticipants(){
                    return participants;
                }
        };
    
    public :

        GroupStateElement groupStateElement;

        Participants participants;

        string makeXML(){
            string xml = "<groupstate timestamp=\"" + groupStateElement.TIMESTAMP  // ex : ”2012-06-13T16:39:57-05:00”
            + "\" lastfocussessionid=\"" + groupStateElement.LASTFOCUSSESSIONID // ex : ”da9274453@company.com”
            + "\" group-type=\"" + groupStateElement.GROUP_TYPE // ex : "closed"
            + "\">";

            for (list<pair<string,string>>::iterator it = participants.getParticipants().begin(); it != participants.getParticipants().end(); it++){
		        xml += "<participant name=\"" + it->first + "\" comm-addr=\"" + it->second + "\"/>";
	        }

            xml += "</groupstate>";

            return xml;
        }

        


};

#endif