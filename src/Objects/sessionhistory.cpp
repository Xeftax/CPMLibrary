#include "sessionhistory.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"


SessionHistory::SessionHistory() : StorageCpmObject("Contribution-ID") {}
SessionHistory::SessionHistory(SessionHistory &copy) : StorageCpmObject(copy) {}


const string& SessionHistory::getObjectType() {
    return objectType;
}

const vector<string>& SessionHistory::getKnownCPMtypeObjects() {
    return knownCPMtypeObjects;
}

void SessionHistory::setSessionInfo(SessionInfo &sessionInfo) {
    this->sessionInfo = make_shared<SessionInfo>(sessionInfo);
}


