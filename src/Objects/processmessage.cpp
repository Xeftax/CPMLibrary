#include "processmessage.h"
#define CPM_MSG_STOR_FROM "From"

string Headers::getCONTENT_TYPE(){
    return CONTENT_TYPE;
}

stringMap Headers::getMap(){
    stringMap content;
    content[CPM_MSG_STOR_FROM]=FROM;
    content["Date:"]=DATE;
    content["Subject:"]=subject;
    content["Conversation-ID:"]=CONVERSATION_ID;
    content["Contribution-ID:"]=CONTRIBUTION_ID;
    content["InReplyTo-Contribution-ID:"]=in_reply_to_contribution_id;
    content["Content-Type:"]=CONTENT_TYPE;
    return content;
}

string Headers::getText(){
    string content;
    content = content + CPM_MSG_STOR_FROM + FROM;
    content = content + "\n" + "Date:" + DATE;
    content = content + "\n" + "Subject:" + subject;
    content = content + "\n" + "Conversation-ID:" + CONVERSATION_ID;
    content = content + "\n" + "Contribution-ID:" + CONTRIBUTION_ID;
    content = content + "\n" + "InReplyTo-Contribution-ID:" + in_reply_to_contribution_id;
    content = content + "\n" + "Content-Type:" + CONTENT_TYPE;
    return content;
}
