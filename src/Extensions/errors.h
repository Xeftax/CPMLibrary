#ifndef ERRORS_H
#define ERRORS_H

#include <exception>
#include <vector>
#include <string>

using namespace std;
 
enum codeError {
    UnlinkedCpmObject = 0,
    LinkedCpmObject = 1,
    IncompatibleCpmObject = 2,
    IncompleteCpmObject = 3,
    WrongPath = 4,
    InconsistentPath = 5,
    TypeUnequality = 6,
    IDsUnequality = 7,
    ContentUnequality = 8,
    CacheUnequality = 9,
    InvalidUID = 10,
    UIDValidityError = 11,
    NameError = 12
};

class Errors: public exception {
    
public:
    Errors(int codeError=-1, string const& errorCplm="") throw();
    Errors(Errors const &copy, string const& errorCplm="") throw();
 
    virtual const char* what() const throw();
    
    int getCode() const throw();
 
private:
    int mCode;
    string mErrorCplm;
    string exceptionMessage;
    
    static vector<string> exceptList;
};

#endif
