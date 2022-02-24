#ifndef PROCESSMESSAGE_H
#define PROCESSMESSAGE_H

#include <list>
#include <sstream>
#include "cpmobject.h"

class XCPM {
public :
    int TYPE;
    list <string> INVITED_PARTICIPANTS;
    string CONTENT_ID;
    string SDP_PARAMETERS;
};

#endif
