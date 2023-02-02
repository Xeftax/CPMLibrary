#include "xcpm.h"
#include "media.h"
#include "errors.h"

XCPM::XCPM(string xcpmContentType, string titleTypeHeader) : CONTENT_TYPE(xcpmContentType), mTitleTypeHeader(titleTypeHeader) {}
XCPM::XCPM(XCPM &copy) : medias(copy.medias){
    headers.setHeaders(copy.headers.getMap());
}

string XCPM::xcpmCompletionExcept = "incomplete XCPM, TYPE and INVITED_PARTICIPANT are mandory parameters";

void XCPM::addMedia(Media &media) {
    media.isComplete();
    medias.insert(make_pair(media.headers.ID, media));
}

string XCPM::format() {
    ostringstream content;
    content << MULTIPART_SEP << endl;
    content << HEADER_CONTENT_TYPE << HEADER_SEP << CONTENT_TYPE << endl << endl;
    content << headers.format() << endl;
    for (auto &media : medias) {
        content << "<media-object>" << endl;
        content << "\t<cid>cid:<" << media.first << "></cid>" << endl;
        content << "\t<sdp>" << endl;
        string sdpCopy = media.second.sdp_parameters;
        int lineEnder;
        while (not sdpCopy.empty()) {
            lineEnder = sdpCopy.find("\n");
            if (lineEnder != string::npos) {
                content << "\t" << sdpCopy.substr(0, lineEnder+1);
                sdpCopy.erase(0,lineEnder+1);
            } else {
                content << "\t" << sdpCopy << endl;
                break;
            }
        }
        content << "\t</sdp>" << endl;
        content << "</media-object>" << endl;
        if (media.first != medias.rbegin()->first) content << endl;
    }
    for (auto &media : medias) {
        content << endl << MULTIPART_SEP << endl;
        content << media.second.format() << endl;
    }
    
    
    return content.str();
}

void XCPM::multipartSpliter(string content) {
    istringstream contentStream(content);

    map<string,string> xcpmRoot;
    map<string,Media> mediaList;
    
    map<string,string> headersRead;
    ostringstream bodyRead;
    
    string lineContent;
    bool bodyContent = false;
    bool rootPart = false;
    while (getline(contentStream,lineContent)){
        if (lineContent == MULTIPART_SEP) {
            if (bodyContent){
                if (rootPart) {
                    xcpmRoot = parsReader(bodyRead.str());
                    rootPart = false;
                } else {
                    string mediaBody = bodyRead.str();
                    Media newMedia(headersRead, mediaBody.substr(0,mediaBody.length()-2));
                    mediaList.insert(make_pair(newMedia.headers.ID,newMedia));
                }
                bodyContent = false;
                headersRead.clear();
                bodyRead.str("");
            }
        } else if (not bodyContent and lineContent == "") {
            bodyContent = true;
        } else {
            if (bodyContent) {
                bodyRead << lineContent << endl;
            } else {
                size_t headerSep = lineContent.find(HEADER_SEP);
                if (headerSep != string::npos) {
                    string header = lineContent.substr(0,headerSep);
                    string content = lineContent.substr(headerSep +sizeof(HEADER_SEP)/sizeof(char) -1);
                    if (header == HEADER_CONTENT_TYPE and content.find("Application/X-CPM-") == 0) {
                        if (xcpmRoot.empty()) rootPart = true;
                        else  ; //error
                    } else headersRead.insert(make_pair(header,content));
                }
            }
        }
    }
    if (rootPart) xcpmRoot = parsReader(bodyRead.str());
    else {
        string mediaBody = bodyRead.str();
        Media newMedia(headersRead, mediaBody.substr(0,mediaBody.length()-1));
        mediaList.insert(make_pair(newMedia.headers.ID,newMedia));
    }
    setXCPM(xcpmRoot, mediaList);
}

map<string,string> XCPM::parsReader(string content) {
    
    map<string,string> headers;
    while (not content.empty()) {
        
        size_t t_begin = content.find("<") +1;
        if (t_begin -1 == string::npos) break;
        size_t t_end = content.find(">");
        string hTitle = content.substr(t_begin,t_end-t_begin);
        string closer = "</"+hTitle.substr(0, hTitle.find_first_of("\n\t "))+">";
        size_t v_begin = t_end +1;
        size_t v_end = content.find(closer);
        string hValue = content.substr(v_begin,v_end-v_begin);
        content.erase(0,v_end+closer.size());
        
        if (headers.find(hTitle) != headers.end()) hTitle += headers.size();
        headers.insert(make_pair(hTitle,hValue));
    }
    
    return headers;
}

void XCPM::setXCPM(map<string,string> &headersBody, map<string,Media> &mediaList) {
    map<string,string> realHeaders;
    for (auto &h : headersBody) {
        if (h.first.find("media-object") == 0) {
            
            map<string,string> sdpHeaders = parsReader(h.second);
            string sdp_param = sdpHeaders.find("sdp")->second;
            sdp_param = sdp_param.substr(2,sdp_param.length() -4);
            size_t t_pos = sdp_param.find("\n\t");
            while (t_pos != string::npos) {
                sdp_param.erase(sdp_param.begin() + t_pos +1);
                t_pos = sdp_param.find("\n\t");
            }
            string content_id = sdpHeaders.find("cid")->second;
            string opener = "cid:<";
            size_t v_begin = content_id.find(opener) + opener.size();
            size_t v_end = content_id.find(">");
            content_id = content_id.substr(v_begin, v_end-v_begin);
            
            auto mediaIt = mediaList.find(content_id);
            if (mediaIt != medias.end()) {
                mediaIt->second.sdp_parameters = sdp_param;
                addMedia(mediaIt->second);
            } else {
                //error
            }
        } else
            realHeaders.insert(make_pair(h.first, h.second));
    }
    headers.setHeaders(realHeaders);
}
    

bool XCPM::isComplete(){
    return headers.isComplete();;
}
