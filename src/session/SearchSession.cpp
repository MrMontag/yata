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
#include "SearchSession.h"
#include "SessionCommon.h"
#include <yaml-cpp/yaml.h>


const std::string SearchSession::EXPRESSION_KEY = "expression";
const std::string SearchSession::IS_REGEX_KEY = "is-regex";
const std::string SearchSession::IS_CASE_SENSITIVE_KEY = "is-case-sensitive";

YAML::Emitter & operator<<(YAML::Emitter & out, const SearchSession & session)
{
    out << YAML::BeginMap;
    out << YAML::Key << SearchSession::EXPRESSION_KEY << YAML::Value << session.expression;
    out << YAML::Key << SearchSession::IS_REGEX_KEY << YAML::Value << session.isRegex;
    out << YAML::Key << SearchSession::IS_CASE_SENSITIVE_KEY << YAML::Value << session.isCaseSensitive;
    out << YAML::EndMap;
    return out;
}

void operator>>(const YAML::Node & in, SearchSession & session)
{
    session.expression = in[SearchSession::EXPRESSION_KEY].as<std::string>();
    session.isRegex = in[SearchSession::IS_REGEX_KEY].as<bool>();
    session.isCaseSensitive = in[SearchSession::IS_CASE_SENSITIVE_KEY].as<bool>();
}
