#include "groupstate.h"
#include "xcpm.h"
#include "cpmmanager.h"

GroupState::GroupState() {}
GroupState::GroupState(string body) {setGroupState(body);}
GroupState::GroupState(GroupState &copy) : CpmObject(copy), groupStateHistory(copy.groupStateHistory) {
}

bool dateComparator::operator()(const string &a, const string &b) const {
    // timestamp format : yyyy-MM-dd'T'HH:mm:ssXXX
    string timeA = a;
    string timeB = b;
    int size = 5;
    int coeftzA = 0, coeftzB = 0;
    for (int i = 1; i<=6; i++) {
        if (i==2) size = 3;
        int val_a = stoi(timeA);
        int val_b = stoi(timeB);
        timeA.erase(0,size);
        timeB.erase(0,size);
        if (i==4 and timeA.substr(5) != timeB.substr(5)) {
            if (timeA.substr(5,1) == "+") coeftzA = 1;
            else coeftzA = -1;
            if (timeB.substr(5,1) == "+") coeftzB = 1;
            else coeftzB = -1;
        }
        if ((i==4 or i==5) and coeftzA!=0) {
            val_a += stoi(timeA.substr(6))*coeftzA;
            val_b += stoi(timeB.substr(6))*coeftzB;
        }
        if (val_a != val_b) return val_a > val_b;
    }
}

bool groupStateComparator::operator()(const GroupStateHeaders &a, const GroupStateHeaders &b) const {
    dateComparator comparator;
    return comparator(a.TIMESTAMP, b.TIMESTAMP);
}

map<GroupStateHeaders,GroupeStateParticipants,groupStateComparator> GroupState::getHistory() {
    return groupStateHistory;
}

void GroupState::update() {
    if (isComplete()) {
        bool worked = groupStateHistory.insert(make_pair(headers, participants)).second;
        if (not worked) throw Errors(-1,"the date "+headers.TIMESTAMP+" already exist");
    }
    if(mParent.lock()) CpmManager::write(shared_from_this());
}

string GroupState::preview(){
    ostringstream content;
    content << CpmObject::preview();
    for (auto &gs : groupStateHistory) {
        content << endl <<"<groupstate";
        content << gs.first.format() << ">" << endl;
        content << gs.second.format();
        content << "</groupstate>" << endl;
    }
    return content.str();
}

map<string,string> GroupState::reader(string content) {
    map<string,string> headers;
    while (not content.empty()) {
        
        size_t t_end = content.find("=”");
        if (t_end == string::npos) break;
        size_t t_begin = content.find_last_of("\n\t ", t_end) +1;
        string hTitle = content.substr(t_begin,t_end-t_begin);
        size_t v_begin = t_end +4;
        size_t v_end = content.find("”", v_begin);
        string hValue = content.substr(v_begin,v_end-v_begin);
        content.erase(0,v_end +3);
        headers.insert(make_pair(hTitle,hValue));
    }
    
    return headers;
}

void GroupState::setGroupState(string content) {
    map<string,string> groupStates = XCPM::parsReader(content);
    
    for (auto &gs : groupStates) {
        participants.~GroupeStateParticipants();
        new(&participants) GroupeStateParticipants();
        headers.setHeaders(reader(gs.first));
        string participantsList = gs.second;
        while (not participantsList.empty()) {
            size_t p_begin = participantsList.find("<");
            if (p_begin == string::npos) break;
            size_t p_end = participantsList.find("/>");
            map<string,string> p_headers = reader(participantsList.substr(p_begin,p_end-p_begin));
            participants.add(p_headers.find("name")->second, p_headers.find("comm-addr")->second);
            participantsList.erase(0,p_end +2);
        }
        update();
    }
    if (not groupStateHistory.empty()) {
        headers.setHeaders(groupStateHistory.begin()->first.getMap());
        participants.setHeaders(groupStateHistory.begin()->second.getMap());
    }
}

bool GroupState::checkWritingIntegrity() {
    shared_ptr<GroupState> groupState = dynamic_pointer_cast<GroupState>(CpmManager::read(getPath()));
    auto it2 = groupState->groupStateHistory.begin();
    for(auto it1 = groupStateHistory.begin();  it1 != groupStateHistory.end(); ++it1) {
        if (it1->first.getMap() != it2->first.getMap() or it1->second.getMap() != it2->second.getMap())
            throw Errors(ContentUnequality);
        ++it2;
    }
    return CpmObject::checkWritingIntegrity();
}

bool GroupState::isComplete(){
    if (not headers.isComplete()) throw Errors(IncompleteCpmObject, headers.headersCompletionExcept);
    return true;
}


const string& GroupState::getObjectType() {
    return objectType;
}

