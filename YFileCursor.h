/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YFILECURSOR_H
#define YFILECURSOR_H

#include <QtGlobal>
#include <vector>

class QTextCursor;
class YTextDocument;

struct YFileCursor {
    qint64 m_lineAddress;
    int m_charPos;
    int m_length;

    YFileCursor(qint64 linePosition = -1, int lineOffset = -1, int length = -1);
    bool isNull() const;
    QTextCursor qTextCursor(YTextDocument * document, const std::vector<qint64> & lineAddresses) const;
};

#endif // YFILECURSOR_H
