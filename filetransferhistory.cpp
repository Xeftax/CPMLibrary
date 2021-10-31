#include "filetransferhistory.h"

stringMap FileTransferHeaders::getMap(){
    stringMap content = Headers::getMap();
    content["Message-Body:"]=MESSAGE_BODY;
    return content;
}

string FileTransferHeaders::getText(){
    string content = Headers::getText();
    content = content + "\n" + "Message-Body:" + MESSAGE_BODY;
    return content;
}
