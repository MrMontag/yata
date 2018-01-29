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
#ifndef SEARCH_SESSION_H
#define SEARCH_SESSION_H

#include <string>

namespace YAML {
    class Emitter;
    class Node;
}

struct SearchSession {
    std::string expression;
    bool isRegex;
    bool isCaseSensitive;

    static const std::string EXPRESSION_KEY;
    static const std::string IS_REGEX_KEY;
    static const std::string IS_CASE_SENSITIVE_KEY;

    SearchSession(std::string e = std::string(), bool r = false, bool cs = false)
        : expression(e)
        , isRegex(r)
        , isCaseSensitive(cs)
    {
    }
};

YAML::Emitter & operator<<(YAML::Emitter & out, const SearchSession & session);
void operator>>(const YAML::Node & in, SearchSession & session);

#endif
