/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
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

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;

PartialLayout::PartialLayout(TailView * tailView)
    : LayoutStrategy(tailView)
    , m_topScreenLine(0)
    , m_firstVisibleBlock(0)
    , m_firstVisibleBlockLine(0)
    , m_lastFileAddress(0)
{
}

void PartialLayout::onFileChanged()
{
    updateDocumentForPartialLayout(true);
}

void PartialLayout::resizeEvent()
{
    updateDocumentForPartialLayout();
}

int PartialLayout::topScreenLine() const
{
    return m_topScreenLine;
}

void PartialLayout::scrollTo(int newTopLine)
{
    int lineChange = newTopLine - topScreenLine();
    updateDocumentForPartialLayout(false, lineChange);
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

        int line_change = -(view()->numLinesOnScreen()/2);
        updateDocumentForPartialLayout(false, line_change, document()->fileCursor().lineAddress());
    }

    return matchFound;
}

bool PartialLayout::wrapAroundForDocumentSearch() const
{
    return false;
}

void PartialLayout::updateAfterKeyPress()
{
    updateDocumentForPartialLayout();
    view()->viewport()->update();
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

    updateDocumentForPartialLayout(false, line_change);
}

void PartialLayout::updateDocumentForPartialLayout(bool file_changed, int line_change /* = 0 */, qint64 new_line_address /* = -1 */)
{
    FileBlockReader * blockReader = view()->blockReader();
    if(0 == blockReader) { return; }

    const int lines_on_screen = view()->numLinesOnScreen();
    const int visible_lines = lines_on_screen + 1;

    const qint64 bottom_screen_pos = bottomScreenPosition();

    const int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);

    view()->updateScrollBars(approx_lines);

    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    qint64 file_pos = 0;
    if(0 == line_change || new_line_address != -1) {
        m_topScreenLine = 0; // TODO: don't reset if scrollbar didn't move
        m_firstVisibleBlock = 0;
        m_firstVisibleBlockLine = 0;

        if(file_changed && view()->followTail()) {
            file_pos = bottom_screen_pos;
            verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
        } else if(new_line_address != -1) {
            file_pos = new_line_address;
        } else if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
            file_pos = bottom_screen_pos;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }

        file_pos = blockReader->getStartPosition(file_pos, 0);

        // Don't change the line further if we're at the bottom
        if(file_pos > bottom_screen_pos) {
            line_change = 0;
            verticalScrollBar->setSliderPosition(verticalScrollBar->maximum());
        }

        file_pos = std::min(file_pos, bottom_screen_pos);

        updateDocument(file_pos, 0, visible_lines);
    }

    if(line_change != 0)
    {
        file_pos = m_lastFileAddress;
        if(line_change < 0) {
            updateDocument(file_pos, line_change + m_firstVisibleBlock, visible_lines);

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

        } else {
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

            file_pos = blockReader->getStartPosition(file_pos, real_line_count);
            file_pos = std::min(file_pos, bottom_screen_pos);

            updateDocument(file_pos, 0, visible_lines);
        }
        verticalScrollBar->setSliderPosition(m_lastFileAddress / APPROXIMATE_CHARS_PER_LINE);
    }

    view()->viewport()->update();
}

void PartialLayout::updateDocument(qint64 start_pos, qint64 lines_after_start, qint64 num_lines)
{
    QString data;
    std::vector<qint64> lineAddresses;
    m_lastFileAddress = view()->blockReader()->readChunk(
        &data, &lineAddresses, start_pos, lines_after_start, num_lines);
    document()->setText(data, lineAddresses);
    performLayout();
}

qint64 PartialLayout::bottomScreenPosition() const
{
    //TODO: Account for when top line of bottom screen is wrapped.

    FileBlockReader bottomReader(view()->blockReader()->filename());
    QString data;
    std::vector<qint64> lineAddresses;
    const int numLines = view()->numLinesOnScreen();
    bottomReader.readChunk(
        &data, &lineAddresses, bottomReader.size(), -numLines, numLines);
    YTextDocument document;
    document.setText(data, lineAddresses);
    document.layout(view()->width());

    int lines = 0;
    qint64 line_address = 0;

    QTextBlock block = document.document()->lastBlock();
    while(block.isValid()) {
        lines += block.layout()->lineCount();
        if(lines >= numLines) {
            line_address = document.blockAddress(block);
            break;
        }
        block = block.previous();
    }

    // in case YTextDocument::blockAddress() returns -1
    line_address = std::max(line_address, 0LL);

    return line_address;
}
