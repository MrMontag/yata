#ifndef SESSION_IO_H
#define SESSION_IO_H

#include <iostream>

#include "SessionCommon.h"

class ApplicationSession;

class SessionIO {
public:
    ParsingStatus::Enum readSession(ApplicationSession * session, const char * filename);
    ParsingStatus::Enum readSession(ApplicationSession * session, std::istream & in);

    void writeSession(const ApplicationSession & session, const char * filename);
    void writeSession(const ApplicationSession & session, std::ostream & out);

    const ParsingError * parsingError() const;
private:
    std::auto_ptr<ParsingError> m_parsingError;
};
#endif
