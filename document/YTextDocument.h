/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YTEXTDOCUMENT_H
#define YTEXTDOCUMENT_H

#include "BlockDataVector.h"
#include "app/YObjectPointer.h"

#include <QScopedPointer>
#include <QString>
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
    void setFileCursor(const YFileCursor & cursor);

    void startSelect(const QPoint & point);
    void moveSelect(const QPoint & point);

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
