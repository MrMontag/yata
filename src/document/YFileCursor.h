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
#ifndef YFILECURSOR_H
#define YFILECURSOR_H

#include <QtGlobal>
#include <vector>

class QTextCursor;
class YTextDocument;


class YFileCursor {
public:
    static const int NULL_VALUE = -1;

    YFileCursor(qint64 linePosition = NULL_VALUE, int lineOffset = 0, int length = 0);
    bool isNull() const;
    void makeNull();
    QTextCursor qTextCursor(const YTextDocument *document) const;

    qint64 lineAddress() const;
    void setLineAddress(qint64 la);

    int charPos() const;
    void setCharPos(int cp);

    qint64 charAddress() const;

    int length() const;
    void setLength(int l);

    void setSelectionEnd(qint64 end);
    qint64 selectionEnd() const;

private:
    qint64 m_lineAddress;
    int m_charPos;
    int m_length;
};

#endif // YFILECURSOR_H
