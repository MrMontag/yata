/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "PartialLayout.h"

#include "DocumentSearch.h"
#include "FileSearch.h"
#include "FileBlockReader.h"
#include "TailView.h"
#include "YTextDocument.h"

#include <QScrollBar>
#include <QTextBlock>
#include <QTextDocument>
#include <QTextLayout>
#include <QtDebug>

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;

PartialLayout::PartialLayout(TailView * tailView)
    : LayoutStrategy(tailView)
    , m_topScreenLine(0)
    , m_firstVisibleBlock(0)
    , m_firstVisibleBlockLine(0)
    , m_bottomDocument(new YTextDocument())
{
}

bool PartialLayout::onFileChanged(QString * error)
{
    m_blockReader.reset(new FileBlockReader(view()->filename()));
    updateBottomDocument();
    if(!updateView(true)) {
        *error = m_blockReader->errorString();
        return false;
    }
    return true;
}

void PartialLayout::resizeEvent()
{
    updateBottomDocument();
    updateView(false);
}

int PartialLayout::topScreenLine() const
{
    return m_topScreenLine;
}

void PartialLayout::scrollTo(int newTopLine)
{
    int lineChange = newTopLine - topScreenLine();
    scrollBy(lineChange);
}

bool PartialLayout::searchFile(bool isForward)
{
    bool matchFound = false;
    FileSearch fileSearch(view()->filename());
    fileSearch.setCursor(document()->fileCursor());
    DocumentSearch * docSearch = view()->documentSearch();
    fileSearch.setSearchCriteria(
        docSearch->lastSearchString(),
        docSearch->searchWasRegex(),
        docSearch->searchWasCaseSensitive());
    if(fileSearch.searchFile(isForward, true)) {
        matchFound = true;
        YFileCursor cursor(fileSearch.cursor());
        document()->setFileCursor(cursor);

        bool needs_scroll = false;
        updateView(false, document()->fileCursor().lineAddress(), &needs_scroll);
        if(needs_scroll) {
            int line_change = -(view()->numLinesOnScreen()/2);
            scrollBy(line_change);
        }
    }

    return matchFound;
}

bool PartialLayout::wrapAroundForDocumentSearch() const
{
    return false;
}

void PartialLayout::updateAfterKeyPress()
{
    updateView(false);
}

void PartialLayout::vScrollBarAction(int action)
{
    int line_change = 0;
    int page_step = view()->numLinesOnScreen() - PAGE_STEP_OVERLAP;

    switch(action) {
    case QAbstractSlider::SliderSingleStepAdd:
        line_change = 1;
        break;
    case QAbstractSlider::SliderSingleStepSub:
        line_change = -1;
        break;
    case QAbstractSlider::SliderPageStepAdd:
        line_change = page_step;
        break;
    case QAbstractSlider::SliderPageStepSub:
        line_change = -page_step;
        break;
    default:
        break;
    }

    if(line_change) {
        scrollBy(line_change);
    } else {
        updateView(false);
    }
}

void PartialLayout::updateScrollBars()
{
    const qint64 bottom_screen_pos = bottomScreenPosition();
    const int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    view()->updateScrollBars(approx_lines);
}

bool PartialLayout::updateView(bool file_changed, qint64 new_line_address /*=-1*/, bool * needs_scroll /*=0*/)
{
    updateScrollBars();
    const qint64 bottom_screen_pos = bottomScreenPosition();

    m_topScreenLine = 0; // TODO: don't reset if scrollbar didn't move
    m_firstVisibleBlock = 0;
    m_firstVisibleBlockLine = 0;

    QScrollBar * verticalScrollBar = view()->verticalScrollBar();
    qint64 file_pos = 0;
    if(new_line_address != -1) {
        file_pos = new_line_address;
    } else if(file_changed && view()->followTail()) {
        file_pos = bottom_screen_pos;
        verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
    } else if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
        file_pos = bottom_screen_pos;
    } else {
        file_pos = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
    }

    file_pos = m_blockReader->getStartPosition(file_pos, 0);

    // Don't change the line further if we're at the bottom
    if(file_pos > bottom_screen_pos) {
        if(needs_scroll) { *needs_scroll = false; }
        verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
    } else {
        if(needs_scroll) { *needs_scroll = true; }
    }

    file_pos = std::min(file_pos, bottom_screen_pos);

    bool success = updateDocument(file_pos, 0);
    if(success) { view()->viewport()->update(); }
    return success;
}

bool PartialLayout::scrollBy(int line_change)
{
    updateScrollBars();

    const qint64 bottom_screen_pos = bottomScreenPosition();
    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    if(line_change != 0) {
        const std::vector<qint64> & lineAddresses = document()->lineAddresses();
        qint64 file_pos = lineAddresses[0];
        if(line_change < 0) {
            if (!scrollUp(file_pos, line_change)) { return false; }
        } else {
            if (!scrollDown(file_pos, line_change, bottom_screen_pos)) { return false; }
        }
        qint64 currentFileAddress = document()->lineAddresses()[0];
        verticalScrollBar->setSliderPosition(currentFileAddress / APPROXIMATE_CHARS_PER_LINE);
    }

    view()->viewport()->update();
    return true;
}

bool PartialLayout::scrollUp(qint64 file_pos, int line_change)
{
    if(!updateDocument(file_pos, line_change + m_firstVisibleBlock)) {
        return false;
    }

    QTextBlock block = document()->document()->findBlockByNumber(-line_change);
    while(m_firstVisibleBlockLine + line_change < 0) {
        line_change += m_firstVisibleBlockLine + 1;
        block = block.previous();
        m_firstVisibleBlockLine = block.layout()->lineCount() - 1;
    }

    m_firstVisibleBlock = block.blockNumber();
    m_firstVisibleBlockLine += line_change;

    m_topScreenLine = m_firstVisibleBlockLine;
    for(QTextBlock block = document()->document()->firstBlock();
        block.blockNumber() < m_firstVisibleBlock;
        block = block.next()) {

        m_topScreenLine += block.layout()->lineCount();
    }
    return true;
}

bool PartialLayout::scrollDown(qint64 file_pos, int line_change, qint64 bottom_screen_pos)
{
    line_change += m_firstVisibleBlockLine;
    int wrapped_line_count = 0;
    int real_line_count = 0;
    QTextBlock block = document()->document()->findBlockByNumber(m_firstVisibleBlock);
    while(line_change - wrapped_line_count >= block.layout()->lineCount()) {
        wrapped_line_count += block.layout()->lineCount();
        block = block.next();
        real_line_count++;
    }

    m_firstVisibleBlockLine = line_change - wrapped_line_count;
    m_topScreenLine = m_firstVisibleBlockLine;
    m_firstVisibleBlock = 0;

    file_pos = m_blockReader->getStartPosition(file_pos, real_line_count);
    file_pos = std::min(file_pos, bottom_screen_pos);

    if(!updateDocument(file_pos, 0)) {
        return false;
    }
    return true;
}

bool PartialLayout::updateDocument(qint64 start_pos, qint64 lines_after_start)
{
    QString data;
    std::vector<qint64> lineAddresses;
    qint64 num_lines = view()->numLinesOnScreen() + 1;
    bool success = m_blockReader->readChunk(&data, &lineAddresses, start_pos, lines_after_start, num_lines);
    document()->setText(data, lineAddresses);
    performLayout();
    return success;
}

void PartialLayout::updateBottomDocument()
{
    FileBlockReader bottomReader(view()->filename());
    QString data;
    std::vector<qint64> lineAddresses;
    const int numLines = view()->numLinesOnScreen();
    bottomReader.readChunk(
        &data, &lineAddresses, bottomReader.size(), -numLines, numLines);
    m_bottomDocument->setText(data, lineAddresses);
    m_bottomDocument->layout(view()->width());
}

qint64 PartialLayout::bottomScreenPosition() const
{
    //TODO: Account for when top line of bottom screen is wrapped.

    int lines = 0;
    qint64 line_address = 0;

    QTextBlock block = m_bottomDocument->document()->lastBlock();
    while(block.isValid()) {
        lines += block.layout()->lineCount();
        if(lines >= view()->numLinesOnScreen()) {
            line_address = m_bottomDocument->blockAddress(block);
            break;
        }
        block = block.previous();
    }

    // in case YTextDocument::blockAddress() returns -1
    line_address = std::max(line_address, 0LL);

    return line_address;
}
