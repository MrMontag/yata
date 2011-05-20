/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
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

