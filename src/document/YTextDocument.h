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
#ifndef YTEXTDOCUMENT_H
#define YTEXTDOCUMENT_H

#include "BlockDataVector.h"
#include "../YObjectPointer.h"

#include <QScopedPointer>
#include <vector>

class QFont;
class QTextBlock;
class QTextDocument;
class QTextCursor;
class TextColor;
class YFileCursor;

class YTextDocument: public QObject {
    Q_OBJECT

public:
    YTextDocument();
    ~YTextDocument();
    void setText(const QString & text, const std::vector<qint64> & newAddresses);
    void layout(int width);
    void markDirty();

    QTextBlock findBlockAtLayoutLine(int layoutLine, int * closestLayoutPos = 0) const;
    qint64 blockAddress(QTextBlock block) const;

    int numLayoutLines() const;

    YFileCursor find(const QRegExp & regex, const YFileCursor & cursor, bool isForward) const;

    void select(const YFileCursor & cursor);
    void clearSelection();

    const BlockDataVector<qint64> & lineAddresses() const { return m_lineAddresses; }
    const BlockDataVector<int> & blockLayoutLines() const { return m_blockLayoutLines; }
    const BlockDataVector<double> & blockGraphicalPositions() const { return m_blockGraphicalPositions; }

    const YFileCursor & fileCursor() const { return *m_selectedCursor; }

    void startSelect(const QPoint & point);
    void moveSelect(const QPoint & point);
    void copySelected();

    QTextBlock begin() const { return m_document->begin(); }
    QTextBlock end() const { return m_document->end(); }
    QTextBlock lastBlock() const { return m_document->lastBlock(); }
    int characterCount() const { return m_document->characterCount(); }

    const QFont & font() const;

signals:
    void contentsChanged();

private:
    qreal layoutBlock(QTextBlock * textBlock);
    void updateFont();
    void setColors(const YFileCursor * cursor, const TextColor & textColor);
    void setColors(QTextCursor *cursor, const TextColor & textColor);
    int qdocPosition(const QPoint point) const;
    YFileCursor yFileCursor(int docPos) const;
public:
    // TODO: make yFileCursor() private
    YFileCursor yFileCursor(const QTextCursor & qcursor) const;

private:
    YObjectPointer<QTextDocument> m_document;
    QScopedPointer<YFileCursor> m_selectedCursor;

    int m_numLayoutLines;
    BlockDataVector<int> m_blockLayoutLines;
    BlockDataVector<double> m_blockGraphicalPositions;
    BlockDataVector<qint64> m_lineAddresses;
    int m_width;
    bool m_needs_layout;
};

#endif // YTEXTDOCUMENT_H
