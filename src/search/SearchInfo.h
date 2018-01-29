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
#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include <QString>
#include <QObject>
#include <list>

struct SearchCriteria {
    QString expression;
    bool isCaseSensitive;
    bool isRegex;

    SearchCriteria(const QString & e = QString(), bool cs = false, bool re = false)
        : expression(e)
        , isCaseSensitive(cs)
        , isRegex(re)
    {
    }
};

class SearchInfo: public QObject {
    Q_OBJECT
public:
    typedef std::list<SearchCriteria> Container;

    ~SearchInfo();

    static SearchInfo & instance();

    void populateSearchList(Container * searches);

    void acceptNewSearch(const SearchCriteria & s);
    const SearchCriteria & search() const;

    typedef Container::const_iterator const_iterator;

    const_iterator begin() const;
    const_iterator end() const;

signals:
    void newSearch();

private:
    SearchInfo();

private:
    Container m_searches;
};

#endif
