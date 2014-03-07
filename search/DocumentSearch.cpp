/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "DocumentSearch.h"
#include "document/YFileCursor.h"
#include "document/YTextDocument.h"
#include <QTextFormat>

DocumentSearch::DocumentSearch(YTextDocument *document)
    : m_document(document)
    , m_textCursor(new YFileCursor)
{
}

DocumentSearch::~DocumentSearch()
{
}

void DocumentSearch::setCursor(const YFileCursor & cursor)
{
    *m_textCursor = cursor;
}

const YFileCursor & DocumentSearch::cursor() const
{
    return *m_textCursor;
}

bool DocumentSearch::searchDocument(bool isForward)
{
    if(m_textCursor->isNull()) {
        *m_textCursor = YFileCursor();
    }

    QRegExp regex(getRegex());

    YFileCursor match = m_document->find(regex, *m_textCursor, isForward);

    *m_textCursor = match;
    return !match.isNull();
}
