#ifndef FILETRANSFERHISTORY_H
#define FILETRANSFERHISTORY_H

#include "sessioninfo.h"

class FileTransferHistory : public SessionInfo {
    
    public :
    FileTransferHistory();
    FileTransferHistory(map<string,string> headers_map, string body);
    FileTransferHistory(FileTransferHistory &copy);
    
    static const string objectType;
    virtual const string& getContentType();
    
    private :
    static string mContentType;
    virtual const string& getObjectType();
        
};

#endif
