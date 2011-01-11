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
    , m_firstVisibleLayoutLine(0)
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
    return m_firstVisibleLayoutLine;
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
    fileSearch.setCursor(*view()->fileCursor());
    fileSearch.setSearchCriteria(
        view()->documentSearch()->lastSearchString(),
        view()->documentSearch()->searchWasRegex(),
        view()->documentSearch()->searchWasCaseSensitive());
    if(fileSearch.searchFile(isForward, true)) {
        matchFound = true;
        YFileCursor cursor(fileSearch.cursor());
        *view()->fileCursor() = cursor;

        int line_change = -(view()->numLinesOnScreen()/2);
        updateDocumentForPartialLayout(false, line_change, view()->fileCursor()->m_lineAddress);
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
    LayoutStrategy::vScrollBarAction(action);

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

    // TODO: account for wrapped lines when computing bottom_screen_pos
    const qint64 bottom_screen_pos = blockReader->getStartPosition(blockReader->size(), -lines_on_screen);

    const int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);

    // In partial layout, page up/down is handled manually, so just pass 0 for the visibleLines argument.
    view()->updateScrollBars(approx_lines, 0);

    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    qint64 file_pos = 0;
    if(0 == line_change || new_line_address != -1) {
        m_firstVisibleLayoutLine = 0; // TODO: don't reset if scrollbar didn't move
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

        QString data;
        m_lastFileAddress = blockReader->readChunk(&data, &view()->lineAddresses(), file_pos, 0, visible_lines).first;

        view()->setDocumentText(data);
        performLayout();
    }

    if(line_change != 0)
    {
        file_pos = m_lastFileAddress;
        if(line_change < 0) {
            QString data;
            file_pos = blockReader->readChunk(&data, &view()->lineAddresses(), file_pos,
                line_change + m_firstVisibleBlock, visible_lines).first;
            view()->setDocumentText(data);
            performLayout();

            QTextBlock block = view()->document()->document()->findBlockByNumber(-line_change);
            while(m_firstVisibleBlockLine + line_change < 0) {
                line_change += m_firstVisibleBlockLine + 1;
                block = block.previous();
                m_firstVisibleBlockLine = block.layout()->lineCount() - 1;
            }

            m_firstVisibleBlock = block.blockNumber();
            m_firstVisibleBlockLine += line_change;

            m_firstVisibleLayoutLine = m_firstVisibleBlockLine;
            for(QTextBlock block = view()->document()->document()->firstBlock();
                block.blockNumber() < m_firstVisibleBlock;
                block = block.next()) {

                m_firstVisibleLayoutLine += block.layout()->lineCount();
            }

        } else {
            line_change += m_firstVisibleBlockLine;
            int wrapped_line_count = 0;
            int real_line_count = 0;
            QTextBlock block = view()->document()->document()->findBlockByNumber(m_firstVisibleBlock);
            while(line_change - wrapped_line_count >= block.layout()->lineCount()) {
                wrapped_line_count += block.layout()->lineCount();
                block = block.next();
                real_line_count++;
            }

            m_firstVisibleBlockLine = line_change - wrapped_line_count;
            m_firstVisibleLayoutLine = m_firstVisibleBlockLine;
            m_firstVisibleBlock = 0;

            file_pos = blockReader->getStartPosition(file_pos, real_line_count);
            file_pos = std::min(file_pos, bottom_screen_pos);
            QString data;
            file_pos = blockReader->readChunk(&data, &view()->lineAddresses(), file_pos, 0, visible_lines).first;
            view()->setDocumentText(data);
            performLayout();

        }
        m_lastFileAddress = file_pos;
        verticalScrollBar->setSliderPosition(m_lastFileAddress / APPROXIMATE_CHARS_PER_LINE);
    }

    view()->viewport()->update();
}
