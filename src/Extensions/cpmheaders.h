#ifndef HEADERS_H
#define HEADERS_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <log4cxx/logger.h>

#define HEADER_CONTENT_TYPE "Content-Type"
#define HEADER_SEP ": "
#define HEADER_XCPM_PARTICIPANT_SEP ";"

using namespace std;


class header {
    
    public :
    
    header(string title, string &value, int strenght, bool is_mandatory);
    header(string title, list<string> &values, string values_sep, int strenght, bool is_mandatory);
    header(string title, int &value, list<string> &defined_values, int strenght, bool is_mandatory);
    
    string getTitle() const;
    virtual string getValue() const;
    virtual void setValue(string value) const;
    int getStrenght() const;
    bool isMandatory() const;
    
    private :
    int mHeaderType;
    
    string mTitle;
    string &mValue;
    int mStrenght;
    bool mMandatory;
    
    list<string> &mValues;
    string mValuesSep;
    
    int &mIndexValue;
    
    string null_string;
    list<string> null_list;
    int null_int = -1;

};


struct headersComparator {
    bool operator()(const header &a, const header &b) const;
    //static log4cxx::Logger logger;
};

class Headers {
        
    public :
    
    virtual void setHeaders(map<string,string> content);
    virtual map<string,string> getMap() const;
    virtual string format() const;
    
    virtual void add(string h_title, string h_value);
    virtual void clear(string h_title);
    virtual void reset();
    
    virtual bool isComplete();
    static string headersCompletionExcept;
    
    protected :
    
    set<header,headersComparator> headersList;
    list<string> additionalHeadersValues;
    
    static bool titleCompare(const header h, const string title);
        
};

struct ConversationHeaders : public Headers {
    
    ConversationHeaders();
    
    string FROM;
    string DATE;
    string subject;
    string CONTENT_TYPE;
};

struct MessageHeaders : public ConversationHeaders {
        
    MessageHeaders();
            
    string TO;
};

struct SessionInfoHeaders : public ConversationHeaders {
        
    SessionInfoHeaders();
        
    string CONVERSATION_ID;
    string CONTRIBUTION_ID;
    string in_reply_to_contribution_id;
};

struct XcpmHeaders : public Headers {
    
    XcpmHeaders(string titleTypeHeader);
    
    int TYPE = -1;
    list<string> INVITED_PARTICIPANTS;
    
    virtual string format() const;
    
    private:
    static list<string> xcpmTypes;
};

struct MediaHeaders : public Headers{
    
    MediaHeaders();
    
    string TYPE;
    string TRANSFER_ENCODING;
    string ID;
    string description;
    string disposition;
};

struct GroupStateHeaders : public Headers{
    
    GroupStateHeaders();
    GroupStateHeaders(const GroupStateHeaders &copy);
    
    string TIMESTAMP;
    string LASTFOCUSSESSIONID;
    string GROUP_TYPE;
    
    virtual string format() const;
};

struct GroupeStateParticipants : public Headers{
    
    GroupeStateParticipants();
    GroupeStateParticipants(const GroupeStateParticipants &copy);
    
    virtual void add(string name, string comm_addr);
    virtual void clear(string comm_addr);
    
    virtual string format() const;
};

#endif
