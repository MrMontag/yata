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
    , m_firstVisibleBlockLine(0)
    , m_bottomDocument(new YTextDocument())
{
}

bool PartialLayout::onFileChanged(QString * error)
{
    m_blockReader.reset(new FileBlockReader(view()->filename()));
    updateBottomDocument();
    bool success = false;
    if(view()->followTail()) {
        QScrollBar * vScrollBar = view()->verticalScrollBar();
        vScrollBar->setSliderPosition(vScrollBar->maximum());
        success = updateView();
    } else {
        success = updateView(topOfScreen());
    }
    if(!success) {
        *error = m_blockReader->errorString();
        return false;
    }
    return true;
}

void PartialLayout::resizeEvent()
{
    updateBottomDocument();
    updateView(topOfScreen());
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

        bool at_bottom = false;
        updateView(document()->fileCursor().lineAddress(), &at_bottom);
        if(!at_bottom) {
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
    updateView();
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
        updateView();
    }
}

void PartialLayout::updateScrollBars()
{
    const qint64 bottom_screen_pos = bottomScreenPosition();
    const int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    view()->updateScrollBars(approx_lines);
}

bool PartialLayout::updateView(qint64 new_line_address /*=-1*/, bool * at_bottom /*=0*/)
{
    int bottomScreenTopBlockLine = 0;
    const qint64 bottom_screen_pos = bottomScreenPosition(&bottomScreenTopBlockLine);

    QScrollBar * verticalScrollBar = view()->verticalScrollBar();
    qint64 file_pos = 0;
    if(new_line_address != -1) {
        // Scroll bar did not move
        file_pos = new_line_address;
        m_topScreenLine = 0;
        m_firstVisibleBlockLine = 0;
    } else {
        // Scroll bar moved
        if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
            file_pos = bottom_screen_pos;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }
    }

    file_pos = m_blockReader->getStartPosition(file_pos, 0);

    // Don't change the line further if we're at the bottom
    if(file_pos >= bottom_screen_pos) {
        file_pos = bottom_screen_pos;
        m_firstVisibleBlockLine = bottomScreenTopBlockLine;
        m_topScreenLine = bottomScreenTopBlockLine;
        if(at_bottom) { *at_bottom = true; }
        verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
    } else {
        if(at_bottom) { *at_bottom = false; }
    }

    bool success = updateDocument(file_pos, 0);
    if(success) { view()->viewport()->update(); }

    return success;
}

bool PartialLayout::scrollBy(int line_change)
{
    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    if(line_change != 0) {
        if(line_change < 0) {
            if (!scrollUp(topOfScreen(), line_change)) { return false; }
        } else {
            if (!scrollDown(topOfScreen(), line_change)) { return false; }
        }
        verticalScrollBar->setSliderPosition(topOfScreen() / APPROXIMATE_CHARS_PER_LINE);
    }

    view()->viewport()->update();
    return true;
}

bool PartialLayout::scrollUp(qint64 file_pos, int line_change)
{
    QTextBlock firstVisibleBlock = document()->findBlockAtLayoutLine(m_topScreenLine);
    if(!updateDocument(file_pos, line_change + firstVisibleBlock.blockNumber())) {
        return false;
    }

    firstVisibleBlock = document()->document()->findBlockByNumber(-line_change);
    while(m_firstVisibleBlockLine + line_change < 0) {
        line_change += m_firstVisibleBlockLine + 1;
        firstVisibleBlock = firstVisibleBlock.previous();
        if(!firstVisibleBlock.isValid()) { break; }
        m_firstVisibleBlockLine = firstVisibleBlock.layout()->lineCount() - 1;
    }

    if(!firstVisibleBlock.isValid()) { // We're at the top
        m_firstVisibleBlockLine = 0;
        m_topScreenLine = 0;
        return true;
    }

    const int firstVisibleBlockNumber = firstVisibleBlock.blockNumber();
    m_firstVisibleBlockLine += line_change;

    m_topScreenLine = m_firstVisibleBlockLine;
    for(QTextBlock block = document()->document()->firstBlock();
        block.blockNumber() < firstVisibleBlockNumber;
        block = block.next()) {

        m_topScreenLine += block.layout()->lineCount();
    }
    return true;
}

bool PartialLayout::scrollDown(qint64 file_pos, int line_change)
{
    int bottomScreenTopBlockLine = 0;
    const qint64 bottom_screen_pos = bottomScreenPosition(&bottomScreenTopBlockLine);
    line_change += m_firstVisibleBlockLine;
    int wrapped_line_count = 0;
    int real_line_count = 0;
    QTextBlock block = document()->findBlockAtLayoutLine(m_topScreenLine);
    while(line_change - wrapped_line_count >= block.layout()->lineCount()) {
        wrapped_line_count += block.layout()->lineCount();
        block = block.next();
        real_line_count++;
    }

    m_firstVisibleBlockLine = line_change - wrapped_line_count;
    m_topScreenLine = m_firstVisibleBlockLine;

    file_pos = m_blockReader->getStartPosition(file_pos, real_line_count);
    if(file_pos >= bottom_screen_pos) {
        file_pos = bottom_screen_pos;
        m_topScreenLine = bottomScreenTopBlockLine;
    }

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

qint64 PartialLayout::topOfScreen() const
{
    const std::vector<qint64> & lineAddresses = document()->lineAddresses();
    if (lineAddresses.empty()) { return 0; }
    return lineAddresses.front();
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
    m_bottomDocument->layout(view()->viewport()->width());
    updateScrollBars();
}

qint64 PartialLayout::bottomScreenPosition(int * blockLine /*=0*/) const
{
    //TODO: Account for when top line of bottom screen is wrapped.

    int lines = 0;
    qint64 line_address = 0;

    QTextBlock block = m_bottomDocument->document()->lastBlock();
    const int linesOnScreen = view()->numLinesOnScreen();
    while(block.isValid()) {
        lines += block.layout()->lineCount();
        if(lines >= linesOnScreen) {
            line_address = m_bottomDocument->blockAddress(block);
            break;
        }
        block = block.previous();
    }

    if(blockLine) {
        *blockLine = lines - linesOnScreen;
    }

    // in case YTextDocument::blockAddress() returns -1
    line_address = std::max(line_address, 0LL);

    return line_address;
}
