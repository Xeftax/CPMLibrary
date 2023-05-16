#ifndef MEDIA_H
#define MEDIA_H

#include "cpmobject.h"

class Media {
    
    public :
    Media();
    Media(map<string,string> headers_map, string body);
    Media(const Media &copy);
    
    MediaHeaders headers;
    string sdp_parameters;
    string media_body;
    
    string format();
    void reader(string content);
    
    static const string objectType;
    
    virtual bool isComplete();
    
    private :
    
    virtual const string& getObjectType();
    
};

#endif
