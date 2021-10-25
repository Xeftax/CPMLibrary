#include "conversationhistory.h"

void ConversationHistory::add(CpmObject cpmObject) {
    if (typeid(cpmObject)!=typeid(*this)){
        cpmObjectList.push_back(cpmObject);
    } else {
        throw std::invalid_argument( "you can't put a Conversation History object on a Conversation History object." );
    }
    
}
