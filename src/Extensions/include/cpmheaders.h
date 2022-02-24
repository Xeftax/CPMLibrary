#ifndef HEADERS_H
#define HEADERS_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>

#define HEADER_CONTENT_TYPE "Content-Type"
#define HEADER_FROM "From"
#define HEADER_TO "To"
#define HEADER_DATE "Date"
#define HEADER_SUBJECT "Subject"
#define HEADER_CONVERSATION_ID "Conversation-ID"
#define HEADER_CONTRIBUTION_ID "Contribution-ID"
#define HEADER_IN_REPLY_TO_CONTRIBUTION_ID "InReplyTo-Contribution-ID"
#define HEADER_SEP ": "

using namespace std;

struct headersMapComparator {
    static vector<string> headersOrder;
    bool operator()(const std::string& a, const std::string& b) const {
        auto itb = find(headersOrder.begin(), headersOrder.end(), b);
        if (itb == headersOrder.cend()) {
            cout << "warning : '" << b << "' is an unknown header." << endl;
            headersOrder.push_back(b);
            itb = find(headersOrder.begin(), headersOrder.end(), b);
        }
        auto ita = find(headersOrder.begin(), headersOrder.end(), a);
        if (ita == headersOrder.cend()) {
            cout << "warning : '" << a << "' is an unknown header." << endl;
            headersOrder.push_back(a);
            ita = find(headersOrder.begin(), headersOrder.end(), a);
        }
        return ita < itb;
    }
};

typedef map<string,string,headersMapComparator> headersMap;

class Headers {
        
    public :
    
    Headers();
    Headers(Headers &copy);
        
    string FROM;
    string DATE;
    string subject = "";
    string CONTENT_TYPE;
    
    virtual void setHeaders(headersMap content);
    virtual headersMap getMap();
    virtual string toText();
        
    protected:
    
    virtual bool headersCompletion();
    static string headersCompletionExcept;
        
};

class MessageHeaders : public Headers {
    
    public :
    friend class Message;
        
    MessageHeaders();
    MessageHeaders(MessageHeaders &copy);
            
    string TO;
    
    virtual void setHeaders(headersMap content);
    virtual headersMap getMap();
    
    protected:
    virtual bool headersCompletion();
    
};

class SessionInfoHeaders : public Headers{
        
    public :
    
    SessionInfoHeaders();
    SessionInfoHeaders(SessionInfoHeaders &copy);
        
    string CONVERSATION_ID;
    string CONTRIBUTION_ID;
    string in_reply_to_contribution_id = "";
    
    virtual void setHeaders(headersMap content);
    virtual headersMap getMap();
    
    protected:
    virtual bool headersCompletion();
        
};

#endif
