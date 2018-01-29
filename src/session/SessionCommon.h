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
#ifndef SESSION_COMMON_H
#define SESSION_COMMON_H

// TODO: move this to a yaml subdirectory, since it's being used by preferences serialization, not just session.

#include <yaml-cpp/yaml.h>
#include <string>

namespace ParsingStatus {
    enum Enum {
        OK,
        PossibleDataLoss,
        IncompatibleVersion,
        Error,
        FileNotFound
    };
}

struct ParsingError {
    int line;
    int column;
    std::string message;

    ParsingError(int l, int n, const std::string & m)
        : line(l)
        , column(n)
        , message(m)
    {
    }
};

#endif
