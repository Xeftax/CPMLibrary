#ifndef CONVERSATIONISTORY_H
#define CONVERSATIONISTORY_H


#include <iostream>
#include <string>
#include <list>

#include "cpmobject.h"

using namespace std;

class ConversationHistory : CpmObject {
    private :

        string NAME;

        list <CpmObject> cpmObjectList;

    public :

        void add(CpmObject cpmObject){
            //TYPE& dynamic_cast<TYPE&> (compObject);
            if (typeid(cpmObject)!=typeid(*this)){
                cpmObjectList.push_back(cpmObject);
            } else {/*error*/}
        }
};

#endif