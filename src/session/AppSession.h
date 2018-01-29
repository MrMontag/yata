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
#ifndef APPLICATION_SESSION_H
#define APPLICATION_SESSION_H

#include "SessionCommon.h"

#include <memory>
#include <string>
#include <vector>
#include <iostream>


namespace YAML {
    class Emitter;
    struct Mark;
    class Node;
}

struct FileSession;
struct SearchSession;

class AppSession {
public:
    typedef std::vector<int> GContainer;

    static const std::string VERSION_KEY;
    static const std::string SEARCH_KEY;
    static const std::string FILE_INDEX_KEY;
    static const std::string FILE_KEY;
    static const std::string GEOMETRY_KEY;
    static const std::string LAST_OPEN_DIR_KEY;

    static const int VERSION_MAJOR = 1;
    static const int VERSION_MINOR = 0;

    AppSession();
    ~AppSession();
 
    void addFile(const FileSession & fileSession);
    void addSearch(const SearchSession & searchSession);

    size_t fileCount() const;
    const FileSession & fileAt(int index) const;

    size_t searchCount() const;
    const SearchSession & searchAt(int index) const;

    ParsingStatus::Enum status() const { return m_status; }
    void setStatus(ParsingStatus::Enum status) { m_status = status; }

    int currentIndex() const { return m_currentIndex; }
    void setCurrentIndex(int currentIndex) { m_currentIndex = currentIndex; }

    void setGeometry(const GContainer & geometry);
    const GContainer & geometry() const;

    void setLastOpenDirectory(const std::string & dir) { m_lastOpenDir = dir; }
    const std::string & lastOpenDirectory() const { return m_lastOpenDir; }

private:
    std::vector<SearchSession> m_searches;
    std::vector<FileSession> m_files;
    int m_currentIndex;
    std::string m_lastOpenDir;
    ParsingStatus::Enum m_status;
    GContainer m_geometry;
};

YAML::Emitter & operator<<(YAML::Emitter & out, const AppSession & appSession);
void operator>>(const YAML::Node & in, AppSession & appSession);

#endif
