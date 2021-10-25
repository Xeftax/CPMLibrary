#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <map>
#include <cstdint>

#include "cpmobject.h"

using namespace std;


typedef map<string,string> messageMap;

class header_string {
    
public :
    
    //header_string(const header_string &copy) : mAllContent(copy.mAllContent), mHeaderName(copy.mHeaderName){}
    header_string(string header_name, messageMap& all_content, string header_content = "") : mAllContent(all_content), mHeaderName(header_name), mHeaderContent(header_content){mAllContent[mHeaderName] = mHeaderContent;}
    
    header_string& operator=(string const &str);
    
    string getValue() ;
    
    //string assign(string str);
    
    
private:
    
    string mHeaderName;
    string mHeaderContent;
    messageMap& mAllContent;

};

class Message : CpmObject {
    
public :
    
    string getName();
    uint32_t getUID();
    uint32_t getUIDValidity();
    
private :

    string FOLDERNAME;
    uint32_t UID;
    uint32_t UIDVALIDITY;

    void setName(string foldername);
    void setUID(uint32_t uid);
    void setUIDValidity(uint32_t uidvalidity);

};

#endif
