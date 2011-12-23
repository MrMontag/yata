/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YTEXTDOCUMENT_H
#define YTEXTDOCUMENT_H

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

class YTextDocument {
public:
    YTextDocument();
    ~YTextDocument();
    void setText(const QString & text, const std::vector<qint64> & newAddresses);
    void layout(int width);
    void markDirty();

    QTextBlock findBlockAtLayoutLine(int layoutLine, int * closestLayoutPos = 0) const;
    int blockLayoutPosition(QTextBlock block) const;
    int blockLayoutPosition(int blockNumber) const;
    qint64 blockAddress(QTextBlock block) const;

    QTextDocument * document();
    const QTextDocument * document() const;
    int numLayoutLines() const;

    void select(const QTextCursor & cursor);

    const std::vector<qint64> & lineAddresses() const { return m_lineAddresses; }

    const YFileCursor & fileCursor() const { return *m_fileCursor; }
    void setFileCursor(const YFileCursor & cursor);

private:
    int layoutBlock(QTextBlock * textBlock);
    void updateFont();
    void setColors(QTextCursor * cursor, const TextColor & textColor);

private:
    YObjectPointer<QTextDocument> m_document;
    QScopedPointer<QTextCursor> m_selectedCursor;

    int m_numLayoutLines;
    std::vector<int> m_blockLayoutLines;
    std::vector<qint64> m_lineAddresses;
    QScopedPointer<YFileCursor> m_fileCursor;
    int m_width;
    bool m_needs_layout;
};

#endif // YTEXTDOCUMENT_H
