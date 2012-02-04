/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "YTextDocument.h"
#include "YFileCursor.h"
#include "preferences/TextColor.h"
#include "preferences/Preferences.h"

#include <QApplication>
#include <QFontMetrics>
#include <QPalette>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextLayout>

#include <algorithm>

#include <QtDebug>

YTextDocument::YTextDocument():
    m_document(new QTextDocument()),
    m_selectedCursor(new YFileCursor),
    m_numLayoutLines(0),
    m_blockLayoutLines(m_document.data()),
    m_blockGraphicalPositions(m_document.data()),
    m_lineAddresses(m_document.data()),
    m_width(0),
    m_needs_layout(false)
{
    m_document->setUndoRedoEnabled(false);
    m_document->setUseDesignMetrics(true);
    updateFont();
    connect(m_document.data(), SIGNAL(contentsChanged()), SIGNAL(contentsChanged()));
}

YTextDocument::~YTextDocument()
{
}

void YTextDocument::setText(const QString & text, const std::vector<qint64> & newAddresses)
{
    m_document->setPlainText(text);
    m_lineAddresses.assign(newAddresses);

    select(*m_selectedCursor);
    m_needs_layout = true;
}

void YTextDocument::layout(int width)
{
    if(!m_needs_layout && m_width == width) {
        return;
    }

    m_width = width;
    m_numLayoutLines = 0;
    qreal dy = 0;
    m_blockLayoutLines.clear();
    m_blockGraphicalPositions.clear();

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        qreal height = layoutBlock(&block);
        m_blockLayoutLines.push_back(m_numLayoutLines);
        m_blockGraphicalPositions.push_back(dy);
        m_numLayoutLines += block.layout()->lineCount();
        dy += height;
    }

    QTextCursor cursor(m_document.data());
    cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::End, QTextCursor::KeepAnchor);
    setColors(&cursor, Preferences::instance()->normalTextColor());

    select(*m_selectedCursor);

    m_needs_layout = false;
}

void YTextDocument::markDirty()
{
    select(*m_selectedCursor);
    updateFont();
    m_needs_layout = true;
}

void YTextDocument::updateFont()
{
    const QFont & font = Preferences::instance()->font();
    if (font != m_document->defaultFont()) {
        m_document->setDefaultFont(font);
        m_needs_layout = true;
    }
}

qreal YTextDocument::layoutBlock(QTextBlock * textBlock)
{
    QTextLayout * layout = textBlock->layout();
    layout->setFont(m_document->defaultFont());
    QFontMetrics fontMetrics(layout->font());

    qreal height = 0;

    layout->beginLayout();
    while(true) {
        QTextLine line = layout->createLine();
        if(!line.isValid()) { break; }
        line.setLineWidth(m_width);
        height += fontMetrics.leading();
        line.setPosition(QPointF(0, height));
        height += line.height();
    }

    layout->endLayout();
    return height;
}

qint64 YTextDocument::blockAddress(QTextBlock block) const
{
    int blockNumber = block.blockNumber();
    if(blockNumber < 0 || static_cast<unsigned int>(blockNumber) >= m_lineAddresses.size()) {
        return -1;
    }
    return m_lineAddresses.at(blockNumber);
}

int YTextDocument::numLayoutLines() const
{
    return m_numLayoutLines;
}

YFileCursor YTextDocument::find(const QRegExp & regex, const YFileCursor & cursor, bool isForward) const
{
    QTextDocument::FindFlags flags = 0;
    if(!isForward) {
        flags |= QTextDocument::FindBackward;
    }
    QTextCursor qcursor = m_document->find(regex, cursor.qTextCursor(this), flags);
    return yFileCursor(qcursor);
}

void YTextDocument::select(const YFileCursor & cursor)
{
    // Clear current selection first
    setColors(m_selectedCursor.data(), Preferences::instance()->normalTextColor());

    *m_selectedCursor = cursor;
    if(!m_selectedCursor->isNull()) {
        setColors(m_selectedCursor.data(), Preferences::instance()->selectedTextColor());
    }
}

void YTextDocument::clearSelection()
{
    select(YFileCursor());
}

void YTextDocument::setColors(const YFileCursor *cursor, const TextColor & textColor)
{
    QTextCursor qcursor(cursor->qTextCursor(this));
    setColors(&qcursor, textColor);
}

void YTextDocument::setColors(QTextCursor * cursor, const TextColor & textColor)
{
    QTextCharFormat format;
    format.setForeground(textColor.foreground());
    format.setBackground(textColor.background());
    cursor->setCharFormat(format);
}

YFileCursor YTextDocument::yFileCursor(const QTextCursor & qcursor) const
{
    if(qcursor.isNull()) { return YFileCursor(); }
    qint64 topAddress = m_lineAddresses.size() ? m_lineAddresses.at(0) : YFileCursor::NULL_VALUE;
    int anchor = qcursor.anchor();
    int pos = qcursor.position();
    if(anchor > pos) { std::swap(anchor, pos); }
    qint64 cursorAddress = topAddress + anchor;
    qint64 lineAddress = 0;
    m_lineAddresses.findContainingBlock(cursorAddress, &lineAddress);
    YFileCursor ycursor(lineAddress, cursorAddress - lineAddress, pos - anchor);
    return ycursor;
}

int YTextDocument::qdocPosition(const QPoint point) const
{
    double blockTop = 0;
    QTextBlock block = m_blockGraphicalPositions.findContainingBlock(point.y(), &blockTop);
    if(!block.isValid()) { return 0; }
    int lineSpacing = QFontMetrics(block.layout()->font()).lineSpacing();
    int block_y = point.y() - blockTop;
    QTextLine line = block.layout()->lineAt(block_y / lineSpacing);
    int blockPos = line.xToCursor(point.x());
    return block.position() + blockPos;
}

void YTextDocument::setFileCursor(const YFileCursor & cursor)
{
    *m_selectedCursor = cursor;
}

void YTextDocument::startSelect(const QPoint & point)
{
    int docPos = qdocPosition(point);
    QTextCursor cursor(m_document.data());
    cursor.setPosition(docPos);
    select(yFileCursor(cursor));
    m_needs_layout = true;
}

void YTextDocument::moveSelect(const QPoint & point)
{
    qint64 docPos = qdocPosition(point) + m_lineAddresses.at(0);
    m_selectedCursor->setLength(docPos - (m_selectedCursor->lineAddress() + m_selectedCursor->charPos()));
    select(*m_selectedCursor);
    m_needs_layout = true;
}

const QFont & YTextDocument::font() const
{
    return Preferences::instance()->font();
}
