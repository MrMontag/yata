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
#include "FileSession.h"
#include "SessionCommon.h"
#include <yaml-cpp/yaml.h>

const std::string FileSession::PATH_KEY = "path";
const std::string FileSession::ADDRESS_KEY = "address";
const std::string FileSession::FOLLOW_TAIL_KEY = "follow-tail";

YAML::Emitter & operator<<(YAML::Emitter & out, const FileSession & session)
{
    out << YAML::BeginMap;
    out << YAML::Key << FileSession::PATH_KEY << YAML::Value << session.path;
    out << YAML::Key << FileSession::ADDRESS_KEY << YAML::Value << session.address;
    out << YAML::Key << FileSession::FOLLOW_TAIL_KEY << YAML::Value << session.followTail;
    out << YAML::EndMap;
    return out;
}

void operator>>(const YAML::Node & in, FileSession & session)
{
    session.path = in[FileSession::PATH_KEY].as<std::string>();
    session.address = in[FileSession::ADDRESS_KEY].as<long long>();
    session.followTail = in[FileSession::FOLLOW_TAIL_KEY].as<bool>();
}
