#include "cpmheaders.h"


//log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("main"));

header::header(string title, string &value, int strenght, bool is_mandatory) : mTitle(title), mValue(value), mStrenght(strenght), mMandatory(is_mandatory), mValues(null_list), mIndexValue(null_int), mHeaderType(0){}

header::header(string title, list<string> &values, string values_sep, int strenght, bool is_mandatory) : mTitle(title), mValue(null_string), mStrenght(strenght), mMandatory(is_mandatory), mValues(values), mValuesSep(values_sep), mIndexValue(null_int), mHeaderType(1) {}

header::header(string title, int &value, list<string> &defined_values, int strenght, bool is_mandatory) : mTitle(title), mValue(null_string), mStrenght(strenght), mMandatory(is_mandatory), mIndexValue(value), mValues(defined_values), mHeaderType(2) {}

string header::getTitle() const { return mTitle;}
int header::getStrenght() const { return mStrenght;}
bool header::isMandatory() const { return mMandatory;}

string header::getValue() const {
    switch (mHeaderType) {
        case 0:
            return mValue;
            
        case 1: {
            string value;
            for (const string &val : mValues)
                value += val + mValuesSep;
            return value.substr(0,value.rfind(mValuesSep));
        }
        case 2: {
            if (mIndexValue < 0 or mIndexValue > mValues.size()) return "";
            list<string>::iterator it = mValues.begin();
            for(int i = 0; i < mIndexValue; i++) {
                it++;
            }
            return *it;
        }
    }
}

void header::setValue(string value) const{
    switch (mHeaderType) {
        case 0:
            mValue.assign(value);
            break;
            
        case 1: {
            while (not value.empty()) {
                size_t sep_pos = value.find(mValuesSep);
                if (sep_pos != string::npos) {
                    mValues.push_back(value.substr(0,sep_pos));
                    value.erase(0,sep_pos+mValuesSep.size());
                } else {
                    mValues.push_back(value);
                    break;
                }
            }
            break;
        }
        case 2: {
            int index = 0;
            for (string &val : mValues) {
                if (val == value) {
                    break;
                }
                index++;
            }
            mIndexValue = index;
            break;
        }
    }
}


bool headersComparator::operator()(const header &a, const header &b) const {
    return a.getStrenght() < b.getStrenght();
}

string Headers::headersCompletionExcept = "incomplete Header, some headers are mandatory (mandatory headers are defined in capitals)";

ConversationHeaders::ConversationHeaders() {
    headersList.insert(header("From", FROM, 1, true));
    headersList.insert(header("Date", DATE, 3, true));
    headersList.insert(header("Subject", subject, 4, false));
    headersList.insert(header("Content-Type", CONTENT_TYPE, 8, true));
}

MessageHeaders::MessageHeaders() {
    headersList.insert(header("To", TO, 2, true));
}

SessionInfoHeaders::SessionInfoHeaders() {
    headersList.insert(header("Conversation-ID", CONVERSATION_ID, 5, true));
    headersList.insert(header("Contribution-ID", CONTRIBUTION_ID, 6, true));
    headersList.insert(header("InReplyTo-Contribution-ID", in_reply_to_contribution_id, 7, false));
}

XcpmHeaders::XcpmHeaders(string titleTypeHeader) {
    headersList.insert(header(titleTypeHeader, TYPE, xcpmTypes, 9, true));
    headersList.insert(header("invited-participants", INVITED_PARTICIPANTS, HEADER_XCPM_PARTICIPANT_SEP, 10, true));
}

MediaHeaders::MediaHeaders() {
    headersList.insert(header("Content-Type", TYPE, 11, true));
    headersList.insert(header("Content-Transfer-Encoding", TRANSFER_ENCODING, 12, true));
    headersList.insert(header("Content-ID", ID, 13, true));
    headersList.insert(header("Content-Description", description, 14, false));
    headersList.insert(header("Content-Disposition", disposition, 15, false));
}
GroupStateHeaders::GroupStateHeaders() {
    headersList.insert(header("timestamp", TIMESTAMP, 16, true));
    headersList.insert(header("lastfocussessionid", LASTFOCUSSESSIONID, 17, true));
    headersList.insert(header("group-type", GROUP_TYPE, 18, true));
};

GroupStateHeaders::GroupStateHeaders(const GroupStateHeaders &copy) : GroupStateHeaders::GroupStateHeaders() {
    setHeaders(copy.getMap());
}

GroupeStateParticipants::GroupeStateParticipants() {}

GroupeStateParticipants::GroupeStateParticipants(const GroupeStateParticipants &copy) {
    setHeaders(copy.getMap());
}

bool Headers::titleCompare(const header h, const string title) {
  return h.getValue() == title;
}

void Headers::add(string h_title, string h_value) {
    additionalHeadersValues.push_back(h_value);
    headersList.insert(header(h_title, *additionalHeadersValues.rbegin(), 30+additionalHeadersValues.size(), false));
}

void Headers::clear(string h_title) {
    auto header = find_if(headersList.begin(), headersList.end(), bind(titleCompare, placeholders::_1, h_title));
    header->setValue("");
}
void Headers::clear() {
    for (const header &h : headersList) {
        h.setValue("");
    }
}

void Headers::setHeaders(map<string,string> content){
    clear();
    for (const header &h : headersList) {
        auto it = content.find(h.getTitle());
        if (it != content.end()) {
            h.setValue(it->second);
            content.erase(it);
        }
    }
    for (const pair<string,string> &h : content) {
        Headers::add(h.first, h.second);
    }
}

map<string,string> Headers::getMap() const{
    map<string,string> content;
    for (auto &h : headersList) {
        content.insert(make_pair(h.getTitle(), h.getValue()));
    }
    return content;
}

string Headers::format() const{
    ostringstream textContent;
    for (auto &h : headersList){
        if (not h.getValue().empty())
            textContent << h.getTitle() << HEADER_SEP << h.getValue() << endl;
    }
    return textContent.str();
}

bool Headers::isComplete(){
    for (auto &h : headersList) {
        if (h.isMandatory() and h.getValue().empty()) return false;
    }
    return true;
}

list<string> XcpmHeaders::xcpmTypes = {"1-1", "Ad-Hoc", "Pre-Defined"};

string XcpmHeaders::format() const {
    ostringstream textContent;
    for (auto &h : headersList){
        if (not h.getValue().empty()) {
            textContent << "<" << h.getTitle() << ">";
            textContent << h.getValue();
            textContent << "</" << h.getTitle() << ">" << endl;
        }
    }
    return textContent.str();
}

string GroupStateHeaders::format() const {
    ostringstream textContent;
    for (auto &h : headersList){
        if (not h.getValue().empty()) {
            textContent << endl << "\t" <<h.getTitle();
            textContent << "=”" << h.getValue() << "”";
        }
    }
    return textContent.str();
}

void GroupeStateParticipants::add(string name, string comm_addr) {
    Headers::add(comm_addr, name);
}

void GroupeStateParticipants::clear(string comm_addr) {
    Headers::clear(comm_addr);
}

string GroupeStateParticipants::format() const {
    ostringstream textContent;
    for (auto &h : headersList){
        if (not h.getValue().empty()) {
            textContent << "\t<participant name=”" << h.getValue();
            textContent << "” comm-addr=”" << h.getTitle() << "”/>" << endl;
        }
    }
    return textContent.str();
}
