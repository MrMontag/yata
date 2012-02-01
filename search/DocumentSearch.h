/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef DOCUMENTSEARCH_H
#define DOCUMENTSEARCH_H

#include <QScopedPointer>
#include <QString>
#include "BaseSearch.h"

class YTextDocument;
class YFileCursor;

class DocumentSearch: public BaseSearch {
public:
    DocumentSearch(YTextDocument * document);

    ~DocumentSearch();

    bool searchDocument(bool isForward, bool wrapAround = true);

    void setCursor(const YFileCursor & cursor);
    const YFileCursor & cursor() const;

private:
    void resetSearchCursor(bool isTop);
private:
    YTextDocument * m_document;

    QScopedPointer<YFileCursor> m_textCursor;
};

#endif // DOCUMENTSEARCH_H
