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

YFileCursor::YFileCursor(qint64 linePosition /*= -1*/, int lineOffset /*= -1*/, int length /*= -1*/)
    : m_lineAddress(linePosition)
    , m_charPos(lineOffset)
    , m_length(length)
{
}

bool YFileCursor::isNull() const
{
    return m_lineAddress == -1;
}

QTextCursor YFileCursor::qTextCursor(YTextDocument * document, const std::vector<qint64> & lineAddresses) const
{
    if(isNull()) { return QTextCursor(); }

    std::vector<qint64>::const_iterator itr =
        std::lower_bound(lineAddresses.begin(), lineAddresses.end(), m_lineAddress);

    if(itr == lineAddresses.end() || *itr != m_lineAddress) {
        return QTextCursor();
    }

    int blockNum = itr - lineAddresses.begin();

    QTextCursor cursor(document->document()->findBlockByNumber(blockNum));

    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, m_charPos);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, m_length);

    return cursor;
}
