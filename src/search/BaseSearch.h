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
