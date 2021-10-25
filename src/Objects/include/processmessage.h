#ifndef PROCESSMESSAGE_H
#define PROCESSMESSAGE_H

#include <list>
#include <iostream>

#include "message.h"

using namespace std;

class ProcessMessage : Message {

protected :

    messageMap content;

    class HeaderContent {
            
    public :
            
        HeaderContent(const HeaderContent &copy) : mAllContent(copy.mAllContent), m_message_body(copy.m_message_body){}
        HeaderContent(messageMap& headers_content, bool message_body=true) : mAllContent(headers_content),m_message_body(message_body){}
            
        header_string FROM {"From: ", mAllContent};
        header_string DATE {"Date: ", mAllContent};
        header_string subject {"Subject: ", mAllContent};
        header_string CONVERSATION_ID {"Conversation-ID: ", mAllContent};
        header_string CONTRIBUTION_ID {"Contribution-ID: ", mAllContent};
        header_string in_reply_to_contribution_id {"InReplyTo-Contribution-ID: ", mAllContent};
        header_string MESSAGE_BODY {"Message-Body: ", mAllContent};

        string getCONTENT_TYPE();
            
    private:
            
        messageMap& mAllContent;
        bool m_message_body;
        header_string CONTENT_TYPE {"Content-Type: ", mAllContent, "multipart/related;boundary=cpm; type=\"Application/X-CPM-File-Transfer\""};
            
    };

    class XCPMContent {
    public :
        int TYPE;
        list <string> INVITED_PARTICIPANTS;
        string CONTENT_ID;
        string SDP_PARAMETERS;
    };
        
};

#endif
