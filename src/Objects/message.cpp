#include "message.h"
#include "errors.h"

Message::Message() {}
Message::Message(Message &copy) : CpmObject(copy) ,headers(copy.headers), message_body(copy.message_body) {}

const string& Message::getObjectType() {
    return objectType;
}

void Message::write(string path, bool verif) {
    CpmObject::write(path, verif);
    path = path + UIDstoHexString();
    ofstream newFile(path);
    newFile << HEADER_CONTENT_TYPE << HEADER_SEP << getObjectType() << endl;
    newFile << endl << headers.toText();
    newFile << endl << message_body;
    newFile.close();
    
    shared_ptr<CpmObject> compareCpmObject = make_shared<Message>();
    if (verif) writeVerif(compareCpmObject, path);
}

void Message::read(string path) {
    if (headersRead.empty() or bodyRead.empty() or typeRead.empty()) {
        folderPathCheck(path);
        CpmObject::read(path);
    }
    headers.setHeaders(headersRead);
    message_body=bodyRead;
    cleanCache();
}

bool Message::isEqual(shared_ptr<CpmObject> cpmObject) {
    CpmObject::isEqual(cpmObject);
    shared_ptr<Message> message = dynamic_pointer_cast<Message>(cpmObject);
    if (headers.getMap() != message->headers.getMap() or message_body != message->message_body)
        throw Errors(ContentUnequality);
    return true;
}


bool Message::cpmObjectValidity(){
    if (not headers.headersCompletion()) throw Errors(IncompleteCpmObject, headers.headersCompletionExcept);
    CpmObject::cpmObjectValidity();
    return true;
}

