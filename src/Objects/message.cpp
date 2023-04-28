#include "message.h"
#include "errors.h"
#include "cpmmanager.h"

log4cxx::LoggerPtr Message::loggerMessage(Logger::getLogger(objectType));

Message::Message() {}
Message::Message(map<string,string> headers_map, string body) : message_body(body) {
    headers.setHeaders(headers_map);}
Message::Message(Message &copy) : CpmObject(copy), message_body(copy.message_body) {
    headers.setHeaders(copy.headers.getMap());
}

const string& Message::getObjectType() {
    return objectType;
}

string Message::preview() {
    ostringstream content;
    content << CpmObject::preview() << endl;
    content << headers.format() << endl;
    content << message_body;
    return content.str();
}

bool Message::checkWritingIntegrity() {
    shared_ptr<Message> message = dynamic_pointer_cast<Message>(CpmManager::read(getPath()));
    if (preview() != message->preview())
        throw Errors(ContentUnequality);
    return CpmObject::checkWritingIntegrity();
}


bool Message::isComplete(){
    if (not headers.isComplete()) LOG4CXX_INFO(loggerMessage, "check object completion : " << headers.headersCompletionExcept);
    return headers.isComplete();
}

