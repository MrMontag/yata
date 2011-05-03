/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "SearchInfo.h"

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

void SearchInfo::setSearch(const SearchCriteria & s)
{
    m_search = s;
    emit newSearch();
}

const SearchCriteria & SearchInfo::search() const
{
    return m_search;
}
