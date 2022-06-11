#include "sessionhistory.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"
#include "cpmmanager.h"


SessionHistory::SessionHistory() : StorageCpmObject("Contribution-ID") {}
SessionHistory::SessionHistory(SessionHistory &copy) : StorageCpmObject(copy) {}


const string& SessionHistory::getObjectType() {
    return objectType;
}

const vector<string>& SessionHistory::getAllowedCpmObjectTypes() {
    return allowedCpmObjectTypes;
}

void SessionHistory::setSessionInfo(SessionInfo &sessionInfo) {
    this->sessionInfo = make_shared<SessionInfo>(sessionInfo);
}

bool SessionHistory::checkWritingIntegrity() {
    sessionInfo->checkWritingIntegrity();
    return StorageCpmObject::checkWritingIntegrity();
}
bool SessionHistory::isComplete(){
    if (sessionInfo->isComplete()) {
        sessionInfo->mParent = shared_from_this();
        sessionInfo->UID = getNextUID();
        NAME = sessionInfo->headers.CONTRIBUTION_ID;
        return true;
    }
    return false;
}


