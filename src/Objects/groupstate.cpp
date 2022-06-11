#include "groupstate.h"

GroupState::GroupState() {}
GroupState::GroupState(map<string,string> headers_map, string body) {}
GroupState::GroupState(GroupState &copy) : CpmObject(copy) {}

string GroupState::format(){
    ostringstream content;
    content << "<groupstate";
    content << headers.format() << ">" << endl;
    content << participants.format();
    content << "</groupstate>";
    return content.str();
}

const string& GroupState::getObjectType() {
    return objectType;
}
