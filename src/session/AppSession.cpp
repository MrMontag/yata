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
    // TODO review
    std::vector<int> version = in[AppSession::VERSION_KEY].as<std::vector<int>>();
    try {
        if (version.size() == 0) {
            return ParsingStatus::PossibleDataLoss;
        } else if (version.size() != 2) {
            return ParsingStatus::IncompatibleVersion;
        } else if (version[0] > AppSession::VERSION_MAJOR) {
            return ParsingStatus::IncompatibleVersion;
        } else if (version[1] > AppSession::VERSION_MINOR) {
            return ParsingStatus::PossibleDataLoss;
        } else {
            return ParsingStatus::OK;
        }
    } catch(YAML::InvalidScalar &) {
        return ParsingStatus::IncompatibleVersion;
    }
//    std::vector<int> versionVec;
//    if (const YAML::Node * version = in.FindValue(AppSession::VERSION_KEY)) {
//        try {
//            *version >> versionVec;
//            if (versionVec.size() != 2) {
//                return ParsingStatus::IncompatibleVersion;
//            } else if (versionVec[0] > AppSession::VERSION_MAJOR) {
//                return ParsingStatus::IncompatibleVersion;
//            } else if (versionVec[1] > AppSession::VERSION_MINOR) {
//                return ParsingStatus::PossibleDataLoss;
//            } else {
//                return ParsingStatus::OK;
//            }
//        } catch(YAML::InvalidScalar &) {
//            return ParsingStatus::IncompatibleVersion;
//        }
//    } else {
//        return ParsingStatus::PossibleDataLoss;
//    }
}

void operator>>(const YAML::Node & in, AppSession & appSession)
{
    // TODO review
    appSession.setStatus(readVersion(in));
    if (appSession.status() == ParsingStatus::IncompatibleVersion) { return; }

    auto searches = in[AppSession::SEARCH_KEY];
    for(YAML::iterator itr = searches.begin(); itr != searches.end(); ++itr) {
        SearchSession search;
        *itr >> search;
        appSession.addSearch(search);
    }
//    if(const YAML::Node * searches = in.FindValue(AppSession::SEARCH_KEY)) {
//        for(YAML::Iterator itr = searches->begin(); itr != searches->end(); ++itr) {
//            SearchSession search;
//            *itr >> search;
//            appSession.addSearch(search);
//        }
//    }

    appSession.setCurrentIndex(in[AppSession::FILE_INDEX_KEY].as<int>());
    auto files = in[AppSession::FILE_KEY];
    for(YAML::iterator itr = files.begin(); itr != files.end(); ++itr) {
        FileSession file;
        *itr >> file;
        appSession.addFile(file);
    }
//    if(const YAML::Node * files = in.FindValue(AppSession::FILE_KEY)) {
//        for(YAML::Iterator itr = files->begin(); itr != files->end(); ++itr) {
//            FileSession file;
//            *itr >> file;
//            appSession.addFile(file);
//        }
//    }

    appSession.setLastOpenDirectory(in[AppSession::LAST_OPEN_DIR_KEY].as<std::string>());
    appSession.setGeometry(in[AppSession::GEOMETRY_KEY].as<AppSession::GContainer>());
}
