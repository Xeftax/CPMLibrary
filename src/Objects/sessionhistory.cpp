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

void SessionHistory::setGroupState(GroupState &groupState) {
    this->groupState = make_shared<GroupState>(groupState);
}

bool SessionHistory::checkWritingIntegrity() {
    sessionInfo->checkWritingIntegrity();
    return StorageCpmObject::checkWritingIntegrity();
}
bool SessionHistory::isComplete(){
    if (not sessionInfo->isComplete()) return false;
    sessionInfo->mParent = shared_from_this();
    sessionInfo->UID = getNextUID();
    NAME = sessionInfo->headers.CONTRIBUTION_ID;
    groupState->mParent = shared_from_this();
    groupState->UID = getNextUID();
    return true;
}


