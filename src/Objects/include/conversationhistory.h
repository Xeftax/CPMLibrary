#ifndef CONVERSATIONISTORY_H
#define CONVERSATIONISTORY_H


#include <iostream>
#include <string>
#include <list>

#include "cpmobject.h"

using namespace std;


class ConversationHistory : CpmObject {

    void add(CpmObject cpmObject);
    
    
    private :

        string NAME;

        list <CpmObject> cpmObjectList;
};

#endif
