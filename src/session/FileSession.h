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
#ifndef SESSION_H
#define SESSION_H

#include <string>

namespace YAML {
    class Emitter;
    class Node;
}

struct FileSession {
    std::string path;
    long long address;
    bool followTail;

    static const std::string PATH_KEY;
    static const std::string ADDRESS_KEY;
    static const std::string FOLLOW_TAIL_KEY;

    FileSession(const std::string & p = std::string(), long long a = 0, bool f = false)
        : path(p)
        , address(a)
        , followTail(f)
    {
    }
};

YAML::Emitter & operator<<(YAML::Emitter & out, const FileSession & session);
void operator>>(const YAML::Node & in, FileSession & session);
#endif
