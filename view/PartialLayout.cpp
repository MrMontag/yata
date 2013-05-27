/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "PartialLayout.h"

#include "DocumentSearch.h"
#include "FileSearch.h"
#include "FileBlockReader.h"
#include "TailView.h"
#include "document/YTextDocument.h"

#include <QScrollBar>
#include <QTextBlock>
#include <QTextLayout>

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;

PartialLayout::PartialLayout(TailView * tailView):
    m_topScreenLine(0),
    m_bottomDocument(new YTextDocument()),
    m_view(tailView)
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

void PartialLayout::performLayout()
{
    document()->layout(view()->viewport()->width());
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
        document()->select(cursor);

        bool at_bottom = false;
        ScreenPosition newPos(document()->fileCursor().lineAddress(), 0);
        updateView(newPos, &at_bottom);
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
    QScrollBar * vsb = view()->verticalScrollBar();

    if(action == QAbstractSlider::SliderMove && !vsb->isSliderDown()) {
        // The slider moved, but wasn't by a keystroke (since isSliderDown() returned false),
        // so assume a wheel event triggered this action (which gets handled by wheelEvent()),
        // and simply return.
        return;
    }

    if(vsb->sliderPosition() > vsb->maximum()) {
        vsb->setSliderPosition(vsb->maximum());
    }

    if(vsb->sliderPosition() < vsb->minimum()) {
        vsb->setSliderPosition(vsb->minimum());
    }

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
    case QAbstractSlider::SliderToMaximum:
    case QAbstractSlider::SliderToMinimum:
    case QAbstractSlider::SliderMove:
    default:
        line_change = 0;
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
    const ScreenPosition bottom_screen_pos = bottomScreenPosition();
    const int approx_lines = static_cast<int>(bottom_screen_pos.address / APPROXIMATE_CHARS_PER_LINE);
    QScrollBar * vsb = view()->verticalScrollBar();
    if(vsb->maximum() != approx_lines) {
        vsb->setRange(0, approx_lines);
        vsb->setPageStep(view()->numLinesOnScreen() - PAGE_STEP_OVERLAP);
        vsb->setSingleStep(1);
    }
}

bool PartialLayout::updateView(ScreenPosition new_line_address /*=ScreenPosition(-1,-1)*/, bool * at_bottom /*=0*/)
{
    const ScreenPosition bottom_screen_pos = bottomScreenPosition();

    QScrollBar * verticalScrollBar = view()->verticalScrollBar();
    qint64 file_pos = 0;
    if(new_line_address.address != -1) {
        // Scroll bar did not move
        file_pos = new_line_address.address;
        m_topScreenLine = new_line_address.blockLine;
    } else {
        // Scroll bar moved
        m_topScreenLine = 0;
        if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
            file_pos = bottom_screen_pos.address;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }
    }

    file_pos = m_blockReader->getStartPosition(file_pos, 0);

    // Don't change the line further if we're at the bottom
    if(file_pos >= bottom_screen_pos.address) {
        file_pos = bottom_screen_pos.address;
        m_topScreenLine = bottom_screen_pos.blockLine;
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
            if (!scrollUp(-line_change)) { return false; }
        } else {
            if (!scrollDown(line_change)) { return false; }
        }
        verticalScrollBar->setSliderPosition(topOfScreen().address / APPROXIMATE_CHARS_PER_LINE + m_topScreenLine);
    }

    view()->viewport()->update();
    return true;
}

bool PartialLayout::scrollUp(int line_change)
{
    if (m_topScreenLine >= line_change) {
        m_topScreenLine -= line_change;
        return true;
    }

    line_change -= m_topScreenLine;
    if(!updateDocument(topOfScreen().address, -line_change)) {
        m_topScreenLine = 0;
        return false;
    }
    QTextBlock firstBlock = document()->begin();
    qint64 file_pos = document()->blockAddress(firstBlock);
    if(file_pos == 0) {
        m_topScreenLine = 0;
    } else {
        int oldTopBlock = document()->blockLayoutLines().at(line_change);
        m_topScreenLine = oldTopBlock - line_change;
    }

    return true;
}

bool PartialLayout::scrollDown(int line_change)
{
    QTextBlock firstBlock = document()->begin();
    if(m_topScreenLine + line_change < firstBlock.layout()->lineCount()) {
        m_topScreenLine += line_change;
        keepToLowerBound(firstBlock, &m_topScreenLine);
        return true;
    }
    int topLineOfNewBlock = 0;
    QTextBlock newTopBlock = document()->blockLayoutLines().findContainingBlock(m_topScreenLine + line_change, &topLineOfNewBlock);
    if(!newTopBlock.isValid()) { return false; }

    m_topScreenLine += line_change - topLineOfNewBlock;
    qint64 file_pos = keepToLowerBound(newTopBlock, &m_topScreenLine);
    return updateDocument(file_pos, 0);
}

qint64 PartialLayout::keepToLowerBound(const QTextBlock & block, int * blockLine)
{
    qint64 file_pos = document()->blockAddress(block);

    const ScreenPosition bottom_screen_pos = bottomScreenPosition();

    if(file_pos >= bottom_screen_pos.address) {
        file_pos = bottom_screen_pos.address;
        if(*blockLine > bottom_screen_pos.blockLine) {
            *blockLine = bottom_screen_pos.blockLine;
        }
    }

    return file_pos;
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

ScreenPosition PartialLayout::topOfScreen() const
{
    const BlockDataVector<qint64> & lineAddresses = document()->lineAddresses();
    qint64 address = lineAddresses.empty() ? 0 : lineAddresses.at(0);
    return ScreenPosition(address, m_topScreenLine);
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
    m_bottomDocument->markDirty();
    m_bottomDocument->layout(view()->viewport()->width());
    updateScrollBars();
}

ScreenPosition PartialLayout::bottomScreenPosition() const
{
    int lines = 0;
    qint64 line_address = 0;

    QTextBlock block = m_bottomDocument->lastBlock();
    const int linesOnScreen = view()->numLinesOnScreen();
    while(block.isValid()) {
        lines += block.layout()->lineCount();
        if(lines >= linesOnScreen) {
            line_address = m_bottomDocument->blockAddress(block);
            break;
        }
        block = block.previous();
    }


    // In case YTextDocument::blockAddress() returns -1 (this could happen if resizing the TailView widget hasn't
    // occurred yet), just return the last character's address so follow tail on start up can happen correctly.
    if(line_address < 0) {
        line_address = m_blockReader->size();
    }

    int blockLine = lines - linesOnScreen;
    ScreenPosition screenPos(line_address, blockLine);
    return screenPos;
}

TailView * PartialLayout::view()
{
    return m_view;
}

TailView * PartialLayout::view() const
{
    return m_view;
}

YTextDocument * PartialLayout::document()
{
    return m_view->document();
}

YTextDocument * PartialLayout::document() const
{
    return m_view->document();
}
