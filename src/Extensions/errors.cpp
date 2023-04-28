#include "errors.h"
#include <sstream>

Errors::Errors(int codeError, string const& errorCplm) throw() : mCode(codeError), mErrorCplm(errorCplm) {
    ostringstream except;
    except << endl << "error : ";
    try {
        except << exceptList.at(mCode);
    } catch(exception const &e) {
        except << "Unknown Type Error";
    }
    mErrorCplm.empty()? except << endl : except << " -> " << mErrorCplm << endl;
    exceptionMessage = except.str();
}
Errors::Errors(Errors const &copy, string const& errorCplm) throw() :Errors::Errors(copy.mCode, errorCplm +" -> "+ copy.mErrorCplm) {}

const char* Errors::what() const throw() {
    return exceptionMessage.c_str();
}

int Errors::getCode() const throw(){
    return mCode;
}

vector<string> exceptList_Init() {
    string c0 = "[Unlinked CPM Object] a CPM Object MUST be linked to a CPM Manager, directly or throught Storage CPM Objects";
    string c1 = "[Already Linked CPM Object] a CPM Object MUST be linked to only one CPM Manager";
    string c2 = "[Incompatible CPM Object Type] check the CPM Objects theorical Organisation/Hierarchy";
    string c3 = "[Incomplete CPM Object] some mandatory parameters of a CPM Object have not been filled in";
    string c4 = "[Wrong Path] check the validity of the path or permissions";
    string c5 = "[Inconsistent Path] the path does not match what is expected";
    string c6 = "[Type Unequality] the two CPM Objects compared do not have the same Type";
    string c7 = "[IDs Unequality] the two CPM Objects compared do not have the same ID";
    string c8 = "[Content Unequality] the two CPM Objects compared do not have the same Content";
    string c9 = "[Cache Content Unequality] the two CPM Objects compared do not have the same Cache Content";
    string c10 = "[Invalid UID] this UID was already assigned";
    string c11 = "[UID Check Error] the CPM Object UID Validity and his parent does not match";
    string c12 = "[Name Check Error] the CPM Object Name contains unallowed characthers";
    
    vector<string> excepts{c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12};
    return excepts;
}

vector<string> Errors::exceptList(exceptList_Init());

