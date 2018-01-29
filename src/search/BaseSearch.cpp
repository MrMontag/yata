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
#include "BaseSearch.h"
#include <QRegExp>

BaseSearch::BaseSearch()
    : m_isSearchRegex(false)
    , m_isSearchCaseSensitive(false)
{
}

const QString & BaseSearch::lastSearchString() const
{
    return m_lastSearchString;
}

bool BaseSearch::searchWasRegex() const
{
    return m_isSearchRegex;
}

bool BaseSearch::searchWasCaseSensitive() const
{
    return m_isSearchCaseSensitive;
}

void BaseSearch::setSearchCriteria(const QString & searchString, bool isRegex, bool caseSensitive)
{
    m_lastSearchString = searchString;
    m_isSearchRegex = isRegex;
    m_isSearchCaseSensitive = caseSensitive;
}

QRegExp BaseSearch::getRegex() const
{
    return QRegExp(lastSearchString(),
        searchWasCaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive,
        searchWasRegex() ? QRegExp::RegExp2 : QRegExp::FixedString);
}
