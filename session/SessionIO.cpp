/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
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
        YAML::Parser parser(in);
        YAML::Node document;
        if (parser.GetNextDocument(document)) {
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
    return m_parsingError.get();
}
