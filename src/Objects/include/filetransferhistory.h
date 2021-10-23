#ifndef FILETRANSFERHISTORY_H
#define FILETRANSFERHISTORY_H

#include "processmessage.h"

enum class FileTransferType {ONE_ONE = 0, AD_HOC = 1, PRE_DEFINED = 2};

class FileTransferHistory : ProcessMessage {
    public :

        HeaderContent headerContent {headersContent};

        messageMap getHeadersContent(){ return headersContent;}

        XCPMContent xCPMContent;
        
};

#endif