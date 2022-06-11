#include "media.h"
#include "errors.h"

Media::Media() {}
Media::Media(map<string,string> headers_map, string body) : media_body(body) {
    headers.setHeaders(headers_map);
}
Media::Media(const Media &copy) : sdp_parameters(copy.sdp_parameters), media_body(copy.media_body) {
    headers.setHeaders(copy.headers.getMap());
}

const string& Media::getObjectType() {
    return objectType;
}

string Media::format() {
    ostringstream content;
    content << headers.format() << endl;
    content << media_body;
    return content.str();
}

void Media::reader(string content) {
    map<string,string> headersRead;
    istringstream text(content);
    string line;
    while(getline(text, line)) {
        size_t headerSep = line.find(HEADER_SEP);
        if (headerSep != string::npos) {
            string title = line.substr(0,headerSep);
            string value = line.substr(headerSep + sizeof(HEADER_SEP)/sizeof(char) -1);
            headersRead.insert(make_pair(title,value));
        } else if (headersRead.size() >= 3) {
            headers.setHeaders(headersRead);
            media_body += line;
        } else throw Errors(IncompatibleCpmObject, "error while reading Media file : the layout of the file does not match with the layout of a CPM Object file");
    }
}

bool Media::isComplete(){
    if (not headers.isComplete()) throw Errors(IncompleteCpmObject, headers.headersCompletionExcept);
    return true;
}
