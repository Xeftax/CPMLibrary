#ifndef MEDIA_H
#define MEDIA_H

#include "cpmobject.h"

class Media : public CpmObject {
    
    public :

    static const string objectType;
    
    private :
    
    virtual const string& getObjectType();
    
};

#endif
