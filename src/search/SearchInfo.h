/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
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
