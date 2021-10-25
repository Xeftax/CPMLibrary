#include "message.h"

header_string& header_string::operator=(string const &str){
    mHeaderContent=str;
    mAllContent[mHeaderName] = mHeaderContent;
    return *this;
};

string header_string::getValue() {
	return mHeaderContent;
}

/*string header_string::assign(string str) {
    return mHeaderContent;
}*/

string Message::getName() { return FOLDERNAME;}
uint32_t Message::getUID() { return UID;}
uint32_t Message::getUIDValidity() { return UIDVALIDITY;}

void Message::setName(string foldername) { FOLDERNAME=foldername;}
void Message::setUID(uint32_t uid) { UID=uid;}
void Message::setUIDValidity(uint32_t uidvalidity) { UIDVALIDITY=uidvalidity;}

