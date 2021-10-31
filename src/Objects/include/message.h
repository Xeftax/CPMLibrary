#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <cstdint>

#include "cpmobject.h"

using namespace std;


class Message : CpmObject {
    
public :
    
    string getName();
    uint32_t getUID();
    uint32_t getUIDValidity();
    
private :

    string FOLDERNAME;
    uint32_t UID;
    uint32_t UIDVALIDITY;

    void setName(string foldername);
    void setUID(uint32_t uid);
    void setUIDValidity(uint32_t uidvalidity);

};

#endif
