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
#include "SessionIO.h"
#include "AppSession.h"

#include <fstream>

ParsingStatus::Enum SessionIO::readSession(AppSession * session, const std::string & filename)
{
    std::ifstream in(filename.c_str());
    if (in) {
        return readSession(session, in);
    }
    return ParsingStatus::FileNotFound;
}

ParsingStatus::Enum SessionIO::readSession(AppSession * session, std::istream & in)
{
    try {
        YAML::Node document = YAML::Load(in);
        if (document.IsDefined()) {
            document >> *session;
        }
    } catch(YAML::Exception & ex) {
        m_parsingError.reset(new ParsingError(ex.mark.line, ex.mark.column, ex.msg));
    }

    return session->status();
}

void SessionIO::writeSession(const AppSession & session, const std::string & filename)
{
    std::ofstream out(filename.c_str());
    if (out) {
        writeSession(session, out);
    }
}

void SessionIO::writeSession(const AppSession & session, std::ostream & out)
{
    YAML::Emitter emitter;
    emitter << session;
    out << emitter.c_str();
}

const ParsingError * SessionIO::parsingError() const
{
    return m_parsingError.data();
}
