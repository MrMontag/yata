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
#include "SearchInfo.h"

const size_t MAX_SEARCH_DEPTH = 500;

SearchInfo::SearchInfo()
{
}

SearchInfo::~SearchInfo()
{
}

SearchInfo & SearchInfo::instance()
{
    static SearchInfo s_instance;
    return s_instance;
}

struct ExpressionEquals {
    QString expression;
    ExpressionEquals(const QString & ex): expression(ex) {}
    bool operator()(const SearchCriteria & sc)
    {
        return sc.expression == expression;
    }
};

void SearchInfo::populateSearchList(Container * searches)
{
    m_searches.clear();
    m_searches.swap(*searches);
}

void SearchInfo::acceptNewSearch(const SearchCriteria & s)
{
    m_searches.remove_if(ExpressionEquals(s.expression));
    while(m_searches.size() >= MAX_SEARCH_DEPTH) {
        m_searches.pop_back();
    }

    m_searches.push_front(s);
    emit newSearch();
}

const SearchCriteria & SearchInfo::search() const
{
    return *begin();
}

SearchInfo::const_iterator SearchInfo::begin() const
{
    return m_searches.begin();
}

SearchInfo::const_iterator SearchInfo::end() const
{
    return m_searches.end();
}

