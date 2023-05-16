#ifndef PROCESSMESSAGE_H
#define PROCESSMESSAGE_H

#include <list>
#include <sstream>
#include "cpmobject.h"

#define MULTIPART_SEP "--cpm"
#define XCPM_PARTICIPANT_SEP "; "
class Media;

class XCPM {
    
    string mTitleTypeHeader;
    
    public :
    friend class SessionInfo;
    friend class FileTransferHistory;
    
    enum Type {ONE_ONE = 0, AD_HOC = 1, PRE_DEFINED = 2};
    
    XCPM(string xcpmContentType, string titleTypeHeader);
    XCPM(XCPM &copy);
    
    XcpmHeaders headers{mTitleTypeHeader};
    
    void addMedia(Media &media);
    
    string format();
    static map<string,string> parsReader(string content);
    void multipartSpliter(string content);
    void setXCPM(map<string,string> &headersBody, map<string,Media> &mediaList);
    
    virtual bool isComplete();
    static string xcpmCompletionExcept;
    
    private :
    map<string,Media> medias;
    static const vector<string> objectTypes;
    string CONTENT_TYPE;

    
    
    
};

#endif
