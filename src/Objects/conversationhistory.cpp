#include "conversationhistory.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"
#include "cpmmanager.h"

ConversationHistory::ConversationHistory(string name) : StorageCpmObject(name) {}
ConversationHistory::ConversationHistory(ConversationHistory &copy) : StorageCpmObject(copy) {}

const string& ConversationHistory::getObjectType() {
    return objectType;
}

const vector<string>& ConversationHistory::getAllowedCpmObjectTypes() {
    return allowedCpmObjectTypes;
}
