#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include "processmessage.h"

enum class SessionInfoType {ONE_ONE = 0, AD_HOC = 1, PRE_DEFINED = 2};

class SessionInfo : CpmObject{
    
public :
            
    Headers headers {"multipart/related;boundary=cpm;type=\"Application/X-CPM-Session\""};

    XCPM xCPM;
    
};

#endif
