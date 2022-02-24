#include "cpmmanager.h"
#include "conversationhistory.h"
#include "sessionhistory.h"
#include "sessioninfo.h"
#include "message.h"
#include "filetransferhistory.h"
#include "media.h"
#include "groupstate.h"


const string CpmObject::objectType = "Unknown/CPIM";

const string StorageCpmObject::objectType = "Unknown/SCPIM";

const string CpmManager::objectType = "CpmManager/SCPIM";

const string ConversationHistory::objectType = "ConversationHistory/SCPIM";

const string SessionHistory::objectType = "SessionHistory/SCPIM";

const string SessionInfo::objectType = "SessionInfo/CPIM";

const string Message::objectType = "Message/CPIM";

const string FileTransferHistory::objectType = "FileTransferHistory/CPIM";

const string GroupState::objectType = "GroupState/CPIM";

const string Media::objectType = "Media/CPIM";

const vector<string> StorageCpmObject::knownCPMtypeObjects = {CpmObject::objectType};

const vector<string> CpmManager::knownCPMtypeObjects = {ConversationHistory::objectType};

const vector<string> ConversationHistory::knownCPMtypeObjects = {SessionHistory::objectType, Message::objectType, FileTransferHistory::objectType, Media::objectType};

const vector<string> SessionHistory::knownCPMtypeObjects = {SessionInfo::objectType, Message::objectType, FileTransferHistory::objectType, GroupState::objectType, Media::objectType};

