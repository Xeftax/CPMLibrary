#ifndef FILETRANSFERHISTORY_H
#define FILETRANSFERHISTORY_H

#include "sessioninfo.h"

enum class FileTransferType {ONE_ONE = 0, AD_HOC = 1, PRE_DEFINED = 2};

class FileTransferHistory : public CpmObject {
    
    public :
    
    SessionInfoHeaders headers;
    
    string MESSAGE_BODY;
    
    XCPM xCPM;
    
    static const string objectType;
    
    private :
    
    virtual const string& getObjectType();
        
};

#endif
