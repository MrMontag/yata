#include "ApplicationSession.h"
#include "FileSession.h"
#include "SearchSession.h"
#include "SessionCommon.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

const std::string ApplicationSession::FILE_KEY = "files";
const std::string ApplicationSession::SEARCH_KEY = "search-data";
const std::string ApplicationSession::VERSION_KEY = "version";

ApplicationSession::ApplicationSession()
    : m_search(new SearchSession)
    , m_status(ParsingStatus::OK)
{
}

ApplicationSession::~ApplicationSession()
{
}


void ApplicationSession::addFile(const FileSession & fileSession)
{
    m_files.push_back(fileSession);
}

void ApplicationSession::setSearch(const SearchSession & searchSession)
{
    m_search.reset(new SearchSession(searchSession));
}

size_t ApplicationSession::fileCount() const
{
    return m_files.size();
}

const FileSession & ApplicationSession::fileAt(int index) const
{
    return m_files.at(index);
}

const SearchSession & ApplicationSession::search() const
{
    return *m_search;
}


YAML::Emitter & operator<<(YAML::Emitter & out, const ApplicationSession & appSession)
{
    out << YAML::BeginMap;

    out << YAML::Key << ApplicationSession::VERSION_KEY << YAML::Value;

    out << YAML::Flow << YAML::BeginSeq;
    out << ApplicationSession::VERSION_MAJOR << ApplicationSession::VERSION_MINOR;
    out << YAML::EndSeq;

    out << YAML::Key << ApplicationSession::SEARCH_KEY << YAML::Value << appSession.search();

    out << YAML::Key << ApplicationSession::FILE_KEY << YAML::Value;
    out << YAML::BeginSeq;
    for(size_t i = 0; i < appSession.fileCount(); i++) {
        out << appSession.fileAt(i);
    }
    out << YAML::EndSeq;

    out << YAML::EndMap;
    return out;
}

ParsingStatus::Enum readVersion(const YAML::Node & in)
{
    std::vector<int> versionVec;
    if (const YAML::Node * version = in.FindValue(ApplicationSession::VERSION_KEY)) {
        try {
            *version >> versionVec;
            if (versionVec.size() != 2) {
                return ParsingStatus::IncompatibleVersion;
            } else if (versionVec[0] > ApplicationSession::VERSION_MAJOR) {
                return ParsingStatus::IncompatibleVersion;
            } else if (versionVec[1] > ApplicationSession::VERSION_MINOR) {
                return ParsingStatus::PossibleDataLoss;
            } else {
                return ParsingStatus::OK;
            }
        } catch(YAML::InvalidScalar &) {
            return ParsingStatus::IncompatibleVersion;
        }
    } else {
        return ParsingStatus::PossibleDataLoss;
    }
}

void operator>>(const YAML::Node & in, ApplicationSession & appSession)
{
    appSession.setStatus(readVersion(in));
    if (appSession.status() == ParsingStatus::IncompatibleVersion) { return; }

    appSession.setSearch(getValue<SearchSession>(in, ApplicationSession::SEARCH_KEY));
    if(const YAML::Node * files = in.FindValue(ApplicationSession::FILE_KEY)) {
        for(YAML::Iterator itr = files->begin(); itr != files->end(); ++itr) {
            FileSession file;
            *itr >> file;
            appSession.addFile(file);
        }
    }
}
