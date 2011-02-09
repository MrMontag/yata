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


class YFileCursor {
public:
    static const int NULL_VALUE = -1;

    YFileCursor(qint64 linePosition = NULL_VALUE, int lineOffset = NULL_VALUE, int length = NULL_VALUE);
    bool isNull() const;
    void makeNull();
    QTextCursor qTextCursor(YTextDocument * document, const std::vector<qint64> & lineAddresses) const;

    qint64 lineAddress() const;
    void setLineAddress(qint64 la);

    int charPos() const;
    void setCharPos(int cp);

    int length() const;
    void setLength(int l);

private:
    qint64 m_lineAddress;
    int m_charPos;
    int m_length;
};

#endif // YFILECURSOR_H
