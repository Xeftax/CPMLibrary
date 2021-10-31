#ifndef FILETRANSFERHISTORY_H
#define FILETRANSFERHISTORY_H

#include "processmessage.h"

enum class FileTransferType {ONE_ONE = 0, AD_HOC = 1, PRE_DEFINED = 2};

class FileTransferHeaders : public Headers{
    
public :
    
    FileTransferHeaders(string content_type) : Headers(content_type) {};
    
    string MESSAGE_BODY;
    
    stringMap getMap();
    
    string getText();
    
};


class FileTransferHistory : CpmObject{
        
public :
	FileTransferHistory() : headers("multipart/related;boundary=cpm;type=\"Application/X-CPM-File-Transfer\""){};
    FileTransferHeaders headers;

    XCPM xCPM;
        
};

#endif
