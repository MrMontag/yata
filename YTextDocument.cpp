#include "YTextDocument.h"

#include <QFontMetrics>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>

#include <algorithm>

YTextDocument::YTextDocument(const QString & text)
    : m_document(new QTextDocument())
    , m_numLayoutLines(0)
    , m_width(0)
    , m_needs_layout(false)
{
    m_document->setUndoRedoEnabled(false);
    if(!text.isEmpty()) {
        setText(text);
    }
}

void YTextDocument::setText(const QString & text)
{
    m_document->setPlainText(text);
    m_needs_layout = true;
}

void YTextDocument::layout(int width)
{
    if(!m_needs_layout && m_width == width) {
        return;
    }

    m_width = width;
    m_numLayoutLines = 0;
    m_layoutPositions.clear();

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        m_layoutPositions.push_back(m_numLayoutLines);
        m_numLayoutLines += layoutBlock(&block);
    }

    m_needs_layout = false;
}

int YTextDocument::layoutBlock(QTextBlock * textBlock)
{
    QTextLayout & layout(*textBlock->layout());
    QFontMetrics fontMetrics(layout.font());

    qreal height = 0;
    int numLines = 0;

    layout.beginLayout();
    while(true) {
        QTextLine line = layout.createLine();
        if(!line.isValid()) { break; }
        numLines++;
        line.setLineWidth(m_width);
        height += fontMetrics.leading();
        line.setPosition(QPointF(0, height));
        height += line.height();
    }

    layout.endLayout();
    return numLines;

}

QTextBlock YTextDocument::findBlockAtLayoutPosition(int layoutPosition, int * closestLayoutPos /* = 0 */)
{
    std::vector<int>::const_iterator blockitr = std::upper_bound(
        m_layoutPositions.begin(),
        m_layoutPositions.end(),
        layoutPosition);
    if(blockitr != m_layoutPositions.begin()) { --blockitr; }
    if(blockitr != m_layoutPositions.end()) {
        if(closestLayoutPos) {
            *closestLayoutPos = *blockitr;
        }
        int blockNumber = blockitr - m_layoutPositions.begin();
        return m_document->findBlockByNumber(blockNumber);
    } else {
        return QTextBlock();
    }
}

int YTextDocument::blockLayoutPosition(QTextBlock block)
{
    int blockNumber = block.blockNumber();
    if(blockNumber < 0 || static_cast<unsigned int>(blockNumber) >= m_layoutPositions.size()) { return -1; }
    return m_layoutPositions[blockNumber];
}

QTextDocument * YTextDocument::document()
{
    return m_document.data();
}

int YTextDocument::numLayoutLines() const
{
    return m_numLayoutLines;
}
