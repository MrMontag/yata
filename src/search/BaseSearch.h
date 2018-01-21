/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef BASESEARCH_H
#define BASESEARCH_H

#include <QString>

class QRegExp;

class BaseSearch
{
public:
    BaseSearch();
    const QString & lastSearchString() const;
    bool searchWasRegex() const;
    bool searchWasCaseSensitive() const;
    void setSearchCriteria(const QString & searchString, bool isRegex, bool caseSensitive);

protected:
    QRegExp getRegex() const;

private:
    QString m_lastSearchString;
    bool m_isSearchRegex;
    bool m_isSearchCaseSensitive;
};

#endif // BASESEARCH_H
