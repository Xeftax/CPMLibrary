#include "filetransferhistory.h"

string FileTransferHistory::mContentType = "multipart/related; boundary=cpm; type=”Application/X-CPM-File-Transfer”";

FileTransferHistory::FileTransferHistory() {
    xcpm.~XCPM(); // destruct
    new(&xcpm) XCPM{"Application/X-CPM-File-Transfer" ,"file-transfer-type"};
}
FileTransferHistory::FileTransferHistory(map<string,string> headers_map, string body) :     FileTransferHistory() {
    headers.setHeaders(headers_map);
    xcpm.multipartSpliter(body);
}
FileTransferHistory::FileTransferHistory(FileTransferHistory &copy) : SessionInfo(copy) {}


const string& FileTransferHistory::getObjectType() {
    return objectType;
}

const string& FileTransferHistory::getContentType() {
    return mContentType;
}
