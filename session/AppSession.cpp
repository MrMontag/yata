/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "AppSession.h"
#include "FileSession.h"
#include "SearchSession.h"
#include "SessionCommon.h"
#include <yaml-cpp/yaml.h>
#include <fstream>

const std::string AppSession::GEOMETRY_KEY = "geometry";
const std::string AppSession::FILE_INDEX_KEY = "current-file-index";
const std::string AppSession::FILE_KEY = "files";
const std::string AppSession::SEARCH_KEY = "search-data";
const std::string AppSession::VERSION_KEY = "version";
const std::string AppSession::LAST_OPEN_DIR_KEY = "last-open-file-directory";

AppSession::AppSession()
    : m_currentIndex(-1)
    , m_status(ParsingStatus::OK)
{
}

AppSession::~AppSession()
{
}


void AppSession::addFile(const FileSession & fileSession)
{
    m_files.push_back(fileSession);
}

void AppSession::addSearch(const SearchSession & searchSession)
{
    m_searches.push_back(searchSession);
}

size_t AppSession::fileCount() const
{
    return m_files.size();
}

const FileSession & AppSession::fileAt(int index) const
{
    return m_files.at(index);
}

size_t AppSession::searchCount() const
{
    return m_searches.size();
}

const SearchSession & AppSession::searchAt(int index) const
{
    return m_searches.at(index);
}

void AppSession::setGeometry(const GContainer & geometry)
{
    m_geometry = geometry;
}

const AppSession::GContainer & AppSession::geometry() const
{
    return m_geometry;
}

YAML::Emitter & operator<<(YAML::Emitter & out, const AppSession & appSession)
{
    out << YAML::BeginMap;

    out << YAML::Key << AppSession::VERSION_KEY << YAML::Value;

    out << YAML::Flow << YAML::BeginSeq;
    out << AppSession::VERSION_MAJOR << AppSession::VERSION_MINOR;
    out << YAML::EndSeq;

    out << YAML::Key << AppSession::SEARCH_KEY << YAML::Value;
    out << YAML::BeginSeq;
    for(size_t i = 0; i < appSession.searchCount(); i++) {
        out << appSession.searchAt(i);
    }
    out << YAML::EndSeq;

    if(appSession.currentIndex() >= 0) {
        out << YAML::Key << AppSession::FILE_INDEX_KEY
            << YAML::Value << appSession.currentIndex();
    }

    out << YAML::Key << AppSession::FILE_KEY << YAML::Value;
    out << YAML::BeginSeq;
    for(size_t i = 0; i < appSession.fileCount(); i++) {
        out << appSession.fileAt(i);
    }
    out << YAML::EndSeq;

    out << YAML::Key << AppSession::LAST_OPEN_DIR_KEY
        << YAML::Value << appSession.lastOpenDirectory();

    out << YAML::Key << AppSession::GEOMETRY_KEY
        << YAML::Value << YAML::Flow << appSession.geometry();

    out << YAML::EndMap;
    return out;
}

ParsingStatus::Enum readVersion(const YAML::Node & in)
{
    std::vector<int> versionVec;
    if (const YAML::Node * version = in.FindValue(AppSession::VERSION_KEY)) {
        try {
            *version >> versionVec;
            if (versionVec.size() != 2) {
                return ParsingStatus::IncompatibleVersion;
            } else if (versionVec[0] > AppSession::VERSION_MAJOR) {
                return ParsingStatus::IncompatibleVersion;
            } else if (versionVec[1] > AppSession::VERSION_MINOR) {
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

void operator>>(const YAML::Node & in, AppSession & appSession)
{
    appSession.setStatus(readVersion(in));
    if (appSession.status() == ParsingStatus::IncompatibleVersion) { return; }

    if(const YAML::Node * searches = in.FindValue(AppSession::SEARCH_KEY)) {
        for(YAML::Iterator itr = searches->begin(); itr != searches->end(); ++itr) {
            SearchSession search;
            *itr >> search;
            appSession.addSearch(search);
        }
    }

    appSession.setCurrentIndex(getValue<int>(in, AppSession::FILE_INDEX_KEY));
    if(const YAML::Node * files = in.FindValue(AppSession::FILE_KEY)) {
        for(YAML::Iterator itr = files->begin(); itr != files->end(); ++itr) {
            FileSession file;
            *itr >> file;
            appSession.addFile(file);
        }
    }

    appSession.setLastOpenDirectory(getValue<std::string>(in, AppSession::LAST_OPEN_DIR_KEY));
    appSession.setGeometry(getValue<AppSession::GContainer>(in, AppSession::GEOMETRY_KEY));
}
