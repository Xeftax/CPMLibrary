#include "sessioninfo.h"
#include "sessionhistory.h"
#include "cpmmanager.h"
#include "errors.h"

string SessionInfo::mContentType = "multipart/related; boundary=cpm; type=”Application/X-CPM-Session”";

SessionInfo::SessionInfo() {
    headers.CONTENT_TYPE = getContentType();
}
SessionInfo::SessionInfo(map<string,string> headers_map, string body) {
    headers.setHeaders(headers_map);
    xcpm.multipartSpliter(body);
}
SessionInfo::SessionInfo(SessionInfo &copy) : CpmObject(copy) , xcpm(copy.xcpm) {
    headers.setHeaders(copy.headers.getMap());
}

const string& SessionInfo::getObjectType() {
    return objectType;
}

const string& SessionInfo::getContentType() {
    return mContentType;
}

string SessionInfo::preview() {
    ostringstream content;
    content << CpmObject::preview() << endl;
    content << headers.format() << endl;
    content << xcpm.format();
    return content.str();
}

bool SessionInfo::checkWritingIntegrity() {
    shared_ptr<SessionInfo> sessionInfo = dynamic_pointer_cast<SessionInfo>(CpmManager::read(getPath()));
    if (headers.getMap() != sessionInfo->headers.getMap() or xcpm.headers.getMap() != sessionInfo->xcpm.headers.getMap() or xcpm.medias.size() != sessionInfo->xcpm.medias.size())
        throw Errors(ContentUnequality);
    
    auto it2 = sessionInfo->xcpm.medias.begin();
    for(auto it1 = xcpm.medias.begin();  it1 != xcpm.medias.end();  ++it1){
        if (it1->second.headers.getMap() != it2->second.headers.getMap() or it1->second.sdp_parameters != it2->second.sdp_parameters or it1->second.media_body != it2->second.media_body)
            throw Errors(ContentUnequality);
        ++it2;
    }
    return CpmObject::checkWritingIntegrity();
}

bool SessionInfo::isComplete(){
    if (not headers.isComplete()) throw Errors(IncompleteCpmObject, headers.headersCompletionExcept);
    if (not xcpm.isComplete()) throw Errors(IncompleteCpmObject, xcpm.xcpmCompletionExcept);
    if (headers.CONTENT_TYPE != getContentType()) {
        cout << "warning : session info content type must be : " << getContentType() << ". It was automatically replaced." << endl;
        headers.CONTENT_TYPE = getContentType();
    }
    return true;
}
