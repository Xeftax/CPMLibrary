#include <cxxtest/TestSuite.h>
#include "message.h"

class MessageTestSuite : public CxxTest::TestSuite {

    public:

        void testClassicConstruction(void) {
            shared_ptr<Message> message;
            TS_ASSERT_THROWS_NOTHING(message = make_shared<Message>());
            checkMessageValues(message, emptyMessageValues);
                      
        }
        
        void testEmptyConstruction(void) {
            shared_ptr<Message> message;
            map<string,string> headers;
            string body;
            TS_ASSERT_THROWS_NOTHING(message = make_shared<Message>(headers,body));
            checkMessageValues(message,emptyMessageValues);
        }

        void testFullConstruction(void) {
            map<string,string> headers = { 
                { "Subject", filledinMessageValues[4] },
                { "Date", filledinMessageValues[3] },
                { "From", filledinMessageValues[1] }, 
                { "Content-Type", filledinMessageValues[5] },
                { "To", filledinMessageValues[2] } 
            };
            string body = "test\ntest";
            shared_ptr<Message> message;
            TS_ASSERT_THROWS_NOTHING(message = make_shared<Message>(headers,body));
            TS_ASSERT_EQUALS(message->isComplete(),true);  
            checkMessageValues(message,filledinMessageValues);
        }   

        void testHeadersFillIn(void) {
            shared_ptr<Message> message;
            TS_ASSERT_THROWS_NOTHING(message = make_shared<Message>());
            
            TS_ASSERT_THROWS_NOTHING(message->headers.TO = filledinMessageValues[2]);
            TS_ASSERT_EQUALS(message->headers.TO, filledinMessageValues[2]);
            TS_ASSERT_EQUALS(message->isComplete(),false); 
            TS_ASSERT_EQUALS(message->preview(),"Content-Type: Message/CPIM\n\nFrom: \nTo: "+filledinMessageValues[2]+"\nDate: \nContent-Type: \n\n");
            
            TS_ASSERT_THROWS_NOTHING(message->headers.FROM = filledinMessageValues[1]);
            TS_ASSERT_EQUALS(message->headers.FROM, filledinMessageValues[1]);
            TS_ASSERT_EQUALS(message->isComplete(),false); 
            TS_ASSERT_EQUALS(message->preview(),"Content-Type: Message/CPIM\n\nFrom: "+filledinMessageValues[1]+"\nTo: "+filledinMessageValues[2]+"\nDate: \nContent-Type: \n\n");
            
            TS_ASSERT_THROWS_NOTHING(message->headers.CONTENT_TYPE = filledinMessageValues[5]);
            TS_ASSERT_EQUALS(message->headers.CONTENT_TYPE, filledinMessageValues[5]);
            TS_ASSERT_EQUALS(message->isComplete(),false); 
            TS_ASSERT_EQUALS(message->preview(),"Content-Type: Message/CPIM\n\nFrom: "+filledinMessageValues[1]+"\nTo: "+filledinMessageValues[2]+"\nDate: \nContent-Type: "+filledinMessageValues[5]+"\n\n");
            
            TS_ASSERT_THROWS_NOTHING(message->headers.DATE = filledinMessageValues[3]);
            TS_ASSERT_EQUALS(message->headers.DATE, filledinMessageValues[3]);
            TS_ASSERT_EQUALS(message->isComplete(),true); 
            TS_ASSERT_EQUALS(message->preview(),"Content-Type: Message/CPIM\n\nFrom: "+filledinMessageValues[1]+"\nTo: "+filledinMessageValues[2]+"\nDate: "+filledinMessageValues[3]+"\nContent-Type: "+filledinMessageValues[5]+"\n\n");

            TS_ASSERT_THROWS_NOTHING(message->headers.subject = filledinMessageValues[4]);
            TS_ASSERT_EQUALS(message->headers.subject, filledinMessageValues[4]);
            TS_ASSERT_EQUALS(message->isComplete(),true); 
            TS_ASSERT_EQUALS(message->preview(),"Content-Type: Message/CPIM\n\nFrom: "+filledinMessageValues[1]+"\nTo: "+filledinMessageValues[2]+"\nDate: "+filledinMessageValues[3]+"\nSubject: "+filledinMessageValues[4]+"\nContent-Type: "+filledinMessageValues[5]+"\n\n");
            
        }

        void testUnlinkedCpmErrors(void) {
            shared_ptr<Message> message;
            TS_ASSERT_THROWS_NOTHING(message = make_shared<Message>());
            TS_ASSERT_THROWS_EQUALS(message->getUID(),const Errors &e, e.getCode(), UnlinkedCpmObject);
            TS_ASSERT_THROWS_EQUALS(message->getUIDValidity(),const Errors &e, e.getCode(), UnlinkedCpmObject);
            TS_ASSERT_THROWS_EQUALS(message->getFolderName(),const Errors &e, e.getCode(), UnlinkedCpmObject);
            TS_ASSERT_THROWS_EQUALS(message->getPath(),const Errors &e, e.getCode(), UnlinkedCpmObject);
        } 

    private:
        vector<string> emptyMessageValues = {"Message/CPIM","","","","","","","Content-Type: Message/CPIM\n\nFrom: \nTo: \nDate: \nContent-Type: \n\n"};
        vector<string> filledinMessageValues = {"Message/CPIM","testfrom@adress.org","testto@adress.org","03/11/2027","test","test","test\ntest","Content-Type: Message/CPIM\n\nFrom: testfrom@adress.org\nTo: testto@adress.org\nDate: 03/11/2027\nSubject: test\nContent-Type: test\n\ntest\ntest"};

        void checkMessageValues(shared_ptr<Message> message, vector<string> values) {
            TS_ASSERT_EQUALS(message->objectType,values[0]);    
            TS_ASSERT_EQUALS(message->headers.FROM,values[1]);
            TS_ASSERT_EQUALS(message->headers.TO,values[2]);    
            TS_ASSERT_EQUALS(message->headers.DATE,values[3]);    
            TS_ASSERT_EQUALS(message->headers.subject,values[4]);   
            TS_ASSERT_EQUALS(message->headers.CONTENT_TYPE,values[5]);
            TS_ASSERT_EQUALS(message->message_body,values[6]);
            TS_ASSERT_EQUALS(message->preview(),values[7]);
        }

};