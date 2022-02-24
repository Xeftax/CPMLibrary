#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include "cpmobject.h"

enum class SessionInfoType {ONE_ONE = 0, AD_HOC = 1, PRE_DEFINED = 2};

class SessionInfo : public CpmObject {
    
    public :
            
    SessionInfoHeaders headers;

    XCPM xCPM;
    
    static const string objectType;
    
    private :
    
    virtual const string& getObjectType();
    
};

#endif
