/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef SEARCHINFO_H
#define SEARCHINFO_H

#include <QString>
#include <QObject>

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
    ~SearchInfo();

    static SearchInfo & instance();

    void setSearch(const SearchCriteria & s);
    const SearchCriteria & search() const;

signals:
    void newSearch();

private:
    SearchInfo();

private:

    SearchCriteria m_search;
};

#endif
