#ifndef PROCESSMESSAGE_H
#define PROCESSMESSAGE_H

#include <list>
#include <iostream>

#include "message.h"

using namespace std;

class ProcessMessage : Message {

    protected :

        messageMap headersContent;

        class HeaderContent {
            private:
                messageMap& m_headers_content;
                bool m_message_body;
                header_string CONTENT_TYPE {"Content-Type: ", m_headers_content, "multipart/related;boundary=cpm; type=\"Application/X-CPM-File-Transfer\""};
            public :
                HeaderContent(const HeaderContent &copy) : m_headers_content(copy.m_headers_content), m_message_body(copy.m_message_body){}
                HeaderContent(messageMap& headers_content, bool message_body=true) : m_headers_content(headers_content), m_message_body(message_body){}
                header_string FROM {"From: ", m_headers_content};
                header_string DATE {"Date: ", m_headers_content};
                header_string subject {"Subject: ", m_headers_content};
                header_string CONVERSATION_ID {"Conversation-ID: ", m_headers_content};
                header_string CONTRIBUTION_ID {"Contribution-ID: ", m_headers_content};
                header_string in_reply_to_contribution_id {"InReplyTo-Contribution-ID: ", m_headers_content};
                header_string MESSAGE_BODY {"Message-Body: ", m_headers_content};

                string getCONTENT_TYPE(){
                    return CONTENT_TYPE.get();
                }     
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