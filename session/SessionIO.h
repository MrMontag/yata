/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef SESSION_IO_H
#define SESSION_IO_H

#include <iostream>
#include <string>

#include "SessionCommon.h"

class AppSession;

class SessionIO {
public:
    ParsingStatus::Enum readSession(AppSession * session, const std::string & filename);
    ParsingStatus::Enum readSession(AppSession * session, std::istream & in);

    void writeSession(const AppSession & session, const std::string & filename);
    void writeSession(const AppSession & session, std::ostream & out);

    const ParsingError * parsingError() const;
private:
    std::auto_ptr<ParsingError> m_parsingError;
};
#endif
