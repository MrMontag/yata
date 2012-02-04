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

void DocumentSearch::resetSearchCursor(bool isTop)
{
    const BlockDataVector<qint64> & lineAddresses = m_document->lineAddresses();
    if(isTop) {
	    *m_textCursor = YFileCursor(lineAddresses.front(), 0, 0);
    } else {
	m_textCursor->setLineAddress(lineAddresses.back());
	m_textCursor->setCharPos(m_document->lastBlock().length() - 1);
	m_textCursor->setLength(0);
    }
}

bool DocumentSearch::searchDocument(bool isForward, bool wrapAround)
{
    if(m_textCursor->isNull()) {
        *m_textCursor = YFileCursor();
    }

    QRegExp regex(getRegex());

    YFileCursor match = m_document->find(regex, *m_textCursor, isForward);

    if(wrapAround && match.isNull()) {
        resetSearchCursor(isForward);
	    match = m_document->find(regex, *m_textCursor, isForward);
    }

    *m_textCursor = match;
    return !match.isNull();
}
