#include "cpmheaders.h"

vector<string> headersMapComparator::headersOrder = {HEADER_FROM, HEADER_TO, HEADER_DATE, HEADER_SUBJECT, HEADER_CONVERSATION_ID, HEADER_CONTRIBUTION_ID, HEADER_IN_REPLY_TO_CONTRIBUTION_ID, HEADER_CONTENT_TYPE};

string Headers::headersCompletionExcept = "incomplete Headers, some headers are mandatory (mandatory headers are defined in capitals)";

Headers::Headers() {}
Headers::Headers(Headers &copy) : FROM(copy.FROM), DATE(copy.DATE), subject(copy.subject), CONTENT_TYPE(copy.CONTENT_TYPE) {}

void Headers::setHeaders(headersMap content){
    FROM=content.find(HEADER_FROM)->second;
    DATE=content.find(HEADER_DATE)->second;
    subject=content.find(HEADER_SUBJECT)->second;
    CONTENT_TYPE=content.find(HEADER_CONTENT_TYPE)->second;
}

headersMap Headers::getMap(){
    headersMap content;
    content[HEADER_FROM]=FROM;
    content[HEADER_DATE]=DATE;
    content[HEADER_SUBJECT]=subject;
    content[HEADER_CONTENT_TYPE]=CONTENT_TYPE;
    return content;
}

string Headers::toText(){
    headersMap mapContent = getMap();
    ostringstream txtContent;
    for (auto& line : mapContent){
        txtContent << line.first << HEADER_SEP << line.second << endl;
    }
    return txtContent.str();
}

bool Headers::headersCompletion(){
    if (FROM.empty() or DATE.empty() or CONTENT_TYPE.empty()) return false;
    return true;
}

MessageHeaders::MessageHeaders() {}
MessageHeaders::MessageHeaders(MessageHeaders &copy) : Headers(copy), TO(copy.TO) {}

void MessageHeaders::setHeaders(headersMap content){
    Headers::setHeaders(content);
    TO=content.find(HEADER_TO)->second;
}

headersMap MessageHeaders::getMap(){
    headersMap content = Headers::getMap();
    content[HEADER_TO]=TO;
    return content;
}

bool MessageHeaders::headersCompletion(){
    if (TO.empty()) return false;
    return Headers::headersCompletion();
}

SessionInfoHeaders::SessionInfoHeaders() {}
SessionInfoHeaders::SessionInfoHeaders(SessionInfoHeaders &copy) : Headers(copy), CONVERSATION_ID(copy.CONVERSATION_ID), CONTRIBUTION_ID(copy.CONTRIBUTION_ID), in_reply_to_contribution_id(copy.in_reply_to_contribution_id) {}

void SessionInfoHeaders::setHeaders(headersMap content){
    Headers::setHeaders(content);
    CONVERSATION_ID=content.find(HEADER_CONVERSATION_ID)->second;
    CONTRIBUTION_ID=content.find(HEADER_CONTRIBUTION_ID)->second;
    in_reply_to_contribution_id=content.find(HEADER_IN_REPLY_TO_CONTRIBUTION_ID)->second;
}

headersMap SessionInfoHeaders::getMap(){
    headersMap content = Headers::getMap();
    content[HEADER_CONVERSATION_ID]=CONVERSATION_ID;
    content[HEADER_CONTRIBUTION_ID]=CONTRIBUTION_ID;
    content[HEADER_IN_REPLY_TO_CONTRIBUTION_ID]=in_reply_to_contribution_id;
    return content;
}

bool SessionInfoHeaders::headersCompletion(){
    if (CONVERSATION_ID.empty() or CONTRIBUTION_ID.empty()) return false;
    return Headers::headersCompletion();
}

