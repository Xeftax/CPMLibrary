#include "message.h"



header_string &header_string::operator=(string const &s){
        m_header_content=s;
            m_headers_content[m_header_name] = m_header_content;
            return *this;
        };

string header_string::get() {
	return m_header_content;
}
