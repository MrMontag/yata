/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SESSION_IO_H
#define SESSION_IO_H

#include <iostream>
#include <string>

#include "SessionCommon.h"
#include <QScopedPointer>

class AppSession;

class SessionIO {
public:
    ParsingStatus::Enum readSession(AppSession * session, const std::string & filename);
    ParsingStatus::Enum readSession(AppSession * session, std::istream & in);

    void writeSession(const AppSession & session, const std::string & filename);
    void writeSession(const AppSession & session, std::ostream & out);

    const ParsingError * parsingError() const;
private:
    QScopedPointer<ParsingError> m_parsingError;
};
#endif
