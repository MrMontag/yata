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
