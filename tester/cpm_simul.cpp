#include "cpmmanager.h"
#include "conversationhistory.h"
#include "sessionhistory.h"
#include "filetransferhistory.h"
#include "groupstate.h"
#include "media.h"
#include "message.h"
#include "sessioninfo.h"


int main() {
    try {
        CpmManager cpmManagerWrite("/Users/quentin/Desktop/CPM message storage/test/", "Simul");
        for (int i = 0; i<10; i++){
            shared_ptr<HistoryCpmObject> conversationHistory = cpmManagerWrite.createNewConversationHistory("CH"+to_string(i));
            shared_ptr<SessionHistory> sessionHistory = make_shared<SessionHistory>();
            conversationHistory->append(sessionHistory);
            for (int j = 0; j<10; j++){
                shared_ptr<Message> message = sessionHistory->createNew<Message>();
                message->headers.FROM = "quentin.monnier"+to_string(i)+"@gmail.com";
                message->headers.TO = "jehan.monnier"+to_string(i)+"@gmail.com";
                message->headers.CONTENT_TYPE = "text/plain; charset=utf-8";
                message->headers.DATE = "2022-01-18T13:40:00-08:0"+to_string(j);
                message->headers.subject = "simulation"+to_string(i);
                message->message_body = "challa ça marche\nc'est le msg n°"+to_string(j);
            }
            
        }
        cpmManagerWrite.write();
        
        CpmManager cpmManagerRead("/Users/quentin/Desktop/CPM message storage/test/", "Simul");
        cpmManagerRead.read();
        
    } catch(string const &e) {
            
        cout << e << endl;
        
    }
    
}

/*
struct Base {
    static const int Value = 5;
    static const string className() { return "Base"; }
    
    Base() {
        cout << "Invoked Base(), Value = " << Value << endl;
    }
    
    virtual void printValue() {
        cout << "Base prints Value of " << Value << endl;
    }
};

struct Derived : public Base {
    
    // Comment me to inherit the base class' Value.
    //
    static const int Value = 10;
    static const string className() { return "Derived"; }
    
    Derived() {
        cout << "Invoked Derived(), Value = " << Value << endl;
    }
    
    // Try to omit this function declaration and see what happens.
    // If you do, does it make a difference whether Value is re-declared?
    //
    virtual void printValue() override {
        cout << "Derived prints Value of " << Value << endl;
    }
};

// Function template accepting both Base and Derived.
//
template <typename T>
void printValue() {
    cout << "Printing " << T::className() << " Value = " << T::Value << endl;
}

int main() {
    // Instantiate a derived class. In the constructor chain,
    // every invocation will look up Value in its own class scope,
    // or parent class', if not present.
    //
    Derived *d = new Derived();
    Base *b1 = new Base();
    Base *b2 = d;
    
    cout << d->Value << endl;
    cout << b1->Value << endl;
    cout << b2->Value << endl;
    
    d->printValue();
    b1->printValue();
    b2->printValue(); // This one is actually a Derived.
    
    // These are actually resolved at compile-time.
    printValue<Base>();
    printValue<Derived>();
    
    return 0;
}*/

