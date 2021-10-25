#include <iostream>

#include "filetransferhistory.h"

using namespace std;

class hstring {
    
public :
    
    hstring(const hstring &copy) : mAllContent(copy.mAllContent), mHeaderName(copy.mHeaderName){
        mAllContent[mHeaderName] = mHeaderContent;}
    hstring(string header_name, messageMap& all_content, string header_content = "") : mAllContent(all_content), mHeaderName(header_name), mHeaderContent(header_content){
        mAllContent[mHeaderName] = mHeaderContent;
    }
    
    hstring& operator=(string str){
        mHeaderContent=str;
        mAllContent[mHeaderName] = mHeaderContent;
        return *this;
    }
    
    hstring& operator=(hstring hstr){
        mHeaderContent=hstr.getValue();
        mAllContent[mHeaderName] = mHeaderContent;
        return *this;
    }

    
    string getValue(){
        return mHeaderContent;
    }
    
    
private:
    
    string mHeaderName;
    string mHeaderContent;
    messageMap& mAllContent;

};

int main(){
    
    
    FileTransferHistory fileTransferHistory;
    fileTransferHistory.headerContent.FROM = "moi";
    fileTransferHistory.headerContent.FROM = "toi";
    fileTransferHistory.headerContent.FROM = "nan je rigole";
    fileTransferHistory.headerContent.DATE = "33";
    fileTransferHistory.headerContent.subject = "oui";
    fileTransferHistory.headerContent.CONVERSATION_ID = "75438292190";
    fileTransferHistory.headerContent.CONTRIBUTION_ID = "32118930281";
    fileTransferHistory.headerContent.in_reply_to_contribution_id = "nop";
    //fileTransferHistory.headerContent.c = "this";
    fileTransferHistory.headerContent.MESSAGE_BODY = "djschxjbsqhhdiscjhvncjxbsdhzbdsodksqsd\ndsfhdjsihfeiozhfjdsbfhds";
    string test = fileTransferHistory.headerContent.FROM.getValue();
    cout << "|" << test << "|\n";
    for (auto& s : fileTransferHistory.getHeadersContent()){
        cout << s.first << s.second << "\n";
    }
    
    return 0;
}
