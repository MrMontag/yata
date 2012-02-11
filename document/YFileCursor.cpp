/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "YFileCursor.h"
#include "YTextDocument.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QTextDocument>
#include <algorithm>

YFileCursor::YFileCursor(qint64 linePosition /*= NULL_VALUE*/, int lineOffset /*= NULL_VALUE*/, int length /*= NULL_VALUE*/)
    : m_lineAddress(linePosition)
    , m_charPos(lineOffset)
    , m_length(length)
{
}

bool YFileCursor::isNull() const
{
    return m_lineAddress == NULL_VALUE;
}

void YFileCursor::makeNull()
{
    m_lineAddress = NULL_VALUE;
    m_charPos = 0;
    m_length = 0;
}

int fileToDocPos(qint64 filePos, const YTextDocument * document)
{
    const BlockDataVector<qint64> & lineAddresses = document->lineAddresses();
    qint64 closest = 0;
    QTextBlock block = lineAddresses.findContainingBlock(filePos, &closest);
    if(!block.isValid()) { return 0; }
    int blockPosition = filePos - closest;
    int docPos = block.position() + blockPosition;
    return std::min(docPos, document->characterCount() - 1);
}

QTextCursor YFileCursor::qTextCursor(const YTextDocument * document) const
{
    if(isNull()) { return QTextCursor(); }

    int anchor = fileToDocPos(charAddress(), document);
    int position = fileToDocPos(selectionEnd(), document);

    if(position < anchor) { std::swap(position, anchor); }

    QTextCursor cursor(document->begin());
    cursor.setPosition(anchor);
    cursor.setPosition(position, QTextCursor::KeepAnchor);

    return cursor;
}

qint64 YFileCursor::lineAddress() const
{
    return m_lineAddress;
}

void YFileCursor::setLineAddress(qint64 la)
{
    m_lineAddress = la;
}

int YFileCursor::charPos() const
{
    return m_charPos;
}

void YFileCursor::setCharPos(int cp)
{
    m_charPos = cp;
}

qint64 YFileCursor::charAddress() const
{
    return m_lineAddress + m_charPos;
}

int YFileCursor::length() const
{
    return m_length;
}

void YFileCursor::setLength(int l)
{
    m_length = l;
}

void YFileCursor::setSelectionEnd(qint64 end)
{
    m_length = end - charAddress();
}

qint64 YFileCursor::selectionEnd() const
{
    return charAddress() + m_length;
}
