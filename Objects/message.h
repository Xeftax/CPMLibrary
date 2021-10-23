#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <map>
#include <cstdint>

#include "cpmobject.h"

using namespace std;

typedef map<string,string> messageMap;

class header_string : public string {
    private:
        string m_header_name;
        string m_header_content;
        messageMap& m_headers_content;
    public :
        header_string(const header_string &copy) : m_headers_content(copy.m_headers_content), m_header_name(copy.m_header_name){}
        header_string(string header_name, messageMap& headers_content, string default_content = "") : m_headers_content(headers_content), m_header_name(header_name), m_header_content(default_content){}
        header_string &operator=(string const &s){
            m_header_content=s;
            m_headers_content[m_header_name] = m_header_content;
            return *this;
        };

        string get() {
            return m_header_content;
        }

};

class Message : CpmObject {

    private :

        string FOLDERNAME;
        uint32_t UID;
        uint32_t UIDVALIDITY;

        void setName(string foldername) { FOLDERNAME=foldername;}
        void setUID(uint32_t uid) { UID=uid;}
        void setUIDValidity(uint32_t uidvalidity) { UIDVALIDITY=uidvalidity;}

    public :

        string getName() { return FOLDERNAME;}
        uint32_t getUID() { return UID;}
        uint32_t getUIDValidity() { return UIDVALIDITY;}

};

#endif