#include "SessionIO.h"
#include "ApplicationSession.h"

#include <fstream>

ParsingStatus::Enum SessionIO::readSession(ApplicationSession * session, const char * filename)
{
    std::ifstream in(filename);
    if (in) {
        return readSession(session, in);
    }
    return ParsingStatus::FileNotFound;
}

ParsingStatus::Enum SessionIO::readSession(ApplicationSession * session, std::istream & in)
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

void SessionIO::writeSession(const ApplicationSession & session, const char * filename)
{
    std::ofstream out(filename);
    if (out) {
        writeSession(session, out);
    }
}

void SessionIO::writeSession(const ApplicationSession & session, std::ostream & out)
{
    YAML::Emitter emitter;
    emitter << session;
    out << emitter.c_str();
}

const ParsingError * SessionIO::parsingError() const
{
    return m_parsingError.get();
}
