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
#include <QtDebug>

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

QTextCursor YFileCursor::qTextCursor(const YTextDocument * document) const
{
    if(isNull()) { return QTextCursor(); }

    const BlockDataVector<qint64> & lineAddresses = document->lineAddresses();
    qint64 docBegin = lineAddresses.front();

    int pos = m_lineAddress + m_charPos - docBegin;
    if(m_length < 0) {
	pos -= -m_length;
    }

    if(pos >= document->characterCount()) {
	return QTextCursor();
    }

    int length = std::abs(m_length);
    if(pos < 0) {
	if (length >= -pos) {
	    length -= -pos;
	    pos = 0;
	} else {
	    return QTextCursor();
	}
    }

    QTextCursor cursor(document->begin());
    cursor.setPosition(pos);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, length);

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

int YFileCursor::length() const
{
    return m_length;
}

void YFileCursor::setLength(int l)
{
    m_length = l;
}

