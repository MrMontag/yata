/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "TailView.h"
#include "FileBlockReader.h"
#include "DocumentSearch.h"
#include "FileSearch.h"
#include "FullLayout.h"
#include "PartialLayout.h"
#include "YApplication.h"
#include "YFileCursor.h"
#include "YFileSystemWatcherThread.h"
#include "YTextDocument.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QString>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextStream>

#include <cmath>
#include <algorithm>

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;
const int PAGE_STEP_OVERLAP = 2;
const qint64 MAX_FULL_LAYOUT_FILE_SIZE = 1024 * 64;

TailView::TailView(QWidget * parent)
    : QAbstractScrollArea(parent)
    , m_document(new YTextDocument)
    , m_fullLayout(false)
    , m_layoutType(AutomaticLayout)
    , m_fullLayoutStrategy(new FullLayout(this))
    , m_partialLayoutStrategy(new PartialLayout(this))
    , m_layoutStrategy(m_partialLayoutStrategy.data())
    , m_followTail(true)
    , m_firstVisibleLayoutLine(0)
    , m_firstVisibleBlock(0)
    , m_firstVisibleBlockLine(0)
    , m_lastFileAddress(0)
    , m_documentSearch(new DocumentSearch(m_document->document()))
    , m_fileCursor(new YFileCursor())
{
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
}

TailView::~TailView()
{
}

void TailView::setFile(const QString & filename)
{
    m_filename = filename;

    if(!filename.isEmpty()) {
        m_watcher.reset(new YFileSystemWatcherThread(filename, this));
        connect(m_watcher.data(), SIGNAL(fileChanged()), SLOT(onFileChanged()));
        connect(m_watcher.data(), SIGNAL(fileDeleted()), SLOT(onFileDeleted()));
    } else {
        m_watcher.reset();
    }

    verticalScrollBar()->setSliderPosition(0);
    horizontalScrollBar()->setSliderPosition(0);

    onFileChanged();
}

void TailView::setLayoutType(LayoutType layoutType)
{
    m_layoutType = layoutType;
    onFileChanged();
}

TailView::LayoutType TailView::layoutType() const
{
    return m_layoutType;
}

const QString & TailView::lastSearchString() const
{
    return m_documentSearch->lastSearchString();
}

bool TailView::searchWasRegex() const
{
    return m_documentSearch->searchWasRegex();
}

bool TailView::searchWasCaseSensitive() const
{
    return m_documentSearch->searchWasCaseSensitive();
}

void TailView::newSearch(const QString & searchString, bool isRegex, bool caseSensitive)
{
    m_documentSearch->setSearchCriteria(searchString, isRegex, caseSensitive);
    searchFile(true);
}

void TailView::searchForward()
{
    searchFile(true);
}

void TailView::searchBackward()
{
    searchFile(false);
}

void TailView::searchFile(bool isForward)
{
    m_layoutStrategy->search(isForward);
}

bool TailView::searchDocument(bool isForward, bool wrapAround)
{
    if(!m_fileCursor->isNull()) {
        m_documentSearch->setCursor(qTextCursor(*m_fileCursor));
    } else {
        const int topLine = verticalScrollBar()->value();
        int layoutLine = 0;
        QTextBlock block = m_document->findBlockAtLayoutPosition(topLine, &layoutLine);
        if(block.isValid()) {
            int blockLine = topLine - layoutLine;
            QTextCursor cursor(block);
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, blockLine);
            m_documentSearch->setCursor(cursor);
        } else {
            m_documentSearch->setCursor(QTextCursor(m_document->document()));
        }
    }

    bool foundMatch = m_documentSearch->searchDocument(isForward, wrapAround);

    if(foundMatch) {
        const QTextCursor & searchCursor = m_documentSearch->cursor();
        m_document->select(searchCursor);
        int blockNum = searchCursor.block().blockNumber();
        m_fileCursor->m_lineAddress = m_lineAddresses[blockNum];
        int cursorBeginPos = std::min(searchCursor.position(), searchCursor.anchor());
        m_fileCursor->m_charPos = cursorBeginPos - searchCursor.block().position();
        m_fileCursor->m_length = std::abs(searchCursor.anchor() - searchCursor.position());
    }

    return foundMatch;
}

void TailView::scrollToIfNecessary(const QTextCursor & cursor)
{
    QTextBlock cursorBlock = cursor.block();

    int cursorLineNumber = m_document->blockLayoutPosition(cursorBlock);

    int blockLine = cursorBlock.layout()->lineForTextPosition(cursor.position() - cursorBlock.position()).lineNumber();

    cursorLineNumber += blockLine;

    int topScreenLine = m_layoutStrategy->topScreenLine();

    int numReadableLines = numLinesOnScreen();

    if(topScreenLine <= cursorLineNumber && cursorLineNumber <= topScreenLine + numReadableLines - 1) {
        // No scrolling needed -- return
        return;
    }

    int newTopLine = cursorLineNumber - numReadableLines / 2;

    m_layoutStrategy->scrollTo(newTopLine);
}

QTextCursor TailView::qTextCursor(const YFileCursor & fileCursor)
{
    if(fileCursor.isNull()) { return QTextCursor(); }

    std::vector<qint64>::const_iterator itr =
        std::lower_bound(m_lineAddresses.begin(), m_lineAddresses.end(), fileCursor.m_lineAddress);

    if(itr == m_lineAddresses.end() || *itr != fileCursor.m_lineAddress) {
        return QTextCursor();
    }

    int blockNum = itr - m_lineAddresses.begin();

    QTextCursor cursor(m_document->document()->findBlockByNumber(blockNum));

    cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, fileCursor.m_charPos);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, fileCursor.m_length);

    return cursor;
}

void TailView::onFileChanged()
{
    m_blockReader.reset(new FileBlockReader(m_filename));

    switch(m_layoutType) {
    case DebugFullLayout: m_fullLayout = true; break;
    case DebugPartialLayout: m_fullLayout = false; break;
    case AutomaticLayout:
        m_fullLayout = (m_blockReader->size() <= MAX_FULL_LAYOUT_FILE_SIZE);
        break;
    }

    if(m_fullLayout) {
        m_layoutStrategy = m_fullLayoutStrategy.data();
    } else {
        m_layoutStrategy = m_partialLayoutStrategy.data();
    }

    m_layoutStrategy->onFileChanged();

    viewport()->update();
}

void TailView::setFollowTail(bool enabled)
{
    m_followTail = enabled;
    onFileChanged();
}

bool TailView::followTail() const
{
    return m_followTail;
}

void TailView::onFileDeleted()
{
    // TODO: display an error message
    m_lineAddresses.clear();
    setDocumentText(QString());
    viewport()->update();
}

void TailView::setDocumentText(const QString & data)
{
    m_document->setText(data);
    m_document->select(qTextCursor(*m_fileCursor));
}

void TailView::performLayout()
{
    m_layoutStrategy->performLayout();
}

void TailView::paintEvent(QPaintEvent * /*event*/)
{
    performLayout();

    qreal dy = 0;

    QTextDocument * document = m_document->document();

    for(QTextBlock block = document->begin(); block != document->end(); block = block.next()) {
        QTextLayout & layout(*block.layout());
        QFontMetrics fontMetrics(layout.font());

        qreal height = fontMetrics.lineSpacing() * layout.lineCount();

        int scrollValue = m_layoutStrategy->topScreenLine();
        QPoint start(0, dy - scrollValue * fontMetrics.lineSpacing());
        QRectF layoutRect(layout.boundingRect());
        layoutRect.moveTo(start);
        QRectF viewrect(viewport()->rect());
        if(viewrect.intersects(layoutRect)) {
            QPainter painter(viewport());
            layout.draw(
                &painter,
                start,
                QVector<QTextLayout::FormatRange>(),
                QRectF(QPointF(0,0), viewport()->size()));
        }
        dy += height;
    }
}

void TailView::resizeEvent(QResizeEvent *)
{
    m_layoutStrategy->resizeEvent();
}

void TailView::keyPressEvent(QKeyEvent * event)
{
    bool handled = false;
    switch(event->key()) {
    case Qt::Key_Home:
        verticalScrollBar()->setValue(0);
        handled = true;
        break;
    case Qt::Key_End:
        verticalScrollBar()->setValue(verticalScrollBar()->maximum());
        handled = true;
        break;
    default:
        QAbstractScrollArea::keyPressEvent(event);
        break;
    }

    if(handled) {
        m_layoutStrategy->updateAfterKeyPress();
    }
}

void TailView::updateDocumentForPartialLayout(bool file_changed, int line_change /* = 0 */, qint64 new_line_address /* = -1 */)
{
    if(m_blockReader.isNull()) { return; }

    const int lines_on_screen = numLinesOnScreen();
    const int visible_lines = lines_on_screen + 1;

    // TODO: account for wrapped lines when computing bottom_screen_pos
    const qint64 bottom_screen_pos = m_blockReader->getStartPosition(m_blockReader->size(), -lines_on_screen);

    const int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    updateScrollBars(approx_lines);

    qint64 file_pos = 0;
    if(0 == line_change || new_line_address != -1) {
        m_firstVisibleLayoutLine = 0; // TODO: don't reset if scrollbar didn't move
        m_firstVisibleBlock = 0;
        m_firstVisibleBlockLine = 0;

        if(file_changed && followTail()) {
            file_pos = bottom_screen_pos;
            verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
        } else if(new_line_address != -1) {
            file_pos = new_line_address;
        } else if(verticalScrollBar()->sliderPosition() >= verticalScrollBar()->maximum()) {
            file_pos = bottom_screen_pos;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar()->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }

        file_pos = m_blockReader->getStartPosition(file_pos, 0);

        // Don't change the line further if we're at the bottom
        if(file_pos > bottom_screen_pos) {
            line_change = 0;
            verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
        }

        file_pos = std::min(file_pos, bottom_screen_pos);

        QString data;
        m_lastFileAddress = m_blockReader->readChunk(&data, &m_lineAddresses, file_pos, 0, visible_lines).first;

        setDocumentText(data);
        performLayout();
    }

    if(line_change != 0)
    {
        file_pos = m_lastFileAddress;
        if(line_change < 0) {
            QString data;
            file_pos = m_blockReader->readChunk(&data, &m_lineAddresses, file_pos,
                line_change + m_firstVisibleBlock, visible_lines).first;
            setDocumentText(data);
            performLayout();

            QTextBlock block = m_document->document()->findBlockByNumber(-line_change);
            while(m_firstVisibleBlockLine + line_change < 0) {
                line_change += m_firstVisibleBlockLine + 1;
                block = block.previous();
                m_firstVisibleBlockLine = block.layout()->lineCount() - 1;
            }

            m_firstVisibleBlock = block.blockNumber();
            m_firstVisibleBlockLine += line_change;

            m_firstVisibleLayoutLine = m_firstVisibleBlockLine;
            for(QTextBlock block = m_document->document()->firstBlock();
                block.blockNumber() < m_firstVisibleBlock;
                block = block.next()) {

                m_firstVisibleLayoutLine += block.layout()->lineCount();
            }

        } else {
            line_change += m_firstVisibleBlockLine;
            int wrapped_line_count = 0;
            int real_line_count = 0;
            QTextBlock block = m_document->document()->findBlockByNumber(m_firstVisibleBlock);
            while(line_change - wrapped_line_count >= block.layout()->lineCount()) {
                wrapped_line_count += block.layout()->lineCount();
                block = block.next();
                real_line_count++;
            }

            m_firstVisibleBlockLine = line_change - wrapped_line_count;
            m_firstVisibleLayoutLine = m_firstVisibleBlockLine;
            m_firstVisibleBlock = 0;

            file_pos = m_blockReader->getStartPosition(file_pos, real_line_count);
            file_pos = std::min(file_pos, bottom_screen_pos);
            QString data;
            file_pos = m_blockReader->readChunk(&data, &m_lineAddresses, file_pos, 0, visible_lines).first;
            setDocumentText(data);
            performLayout();

        }
        m_lastFileAddress = file_pos;
        verticalScrollBar()->setSliderPosition(m_lastFileAddress / APPROXIMATE_CHARS_PER_LINE);
    }

    viewport()->update();
}

void TailView::updateScrollBars(int lines)
{
    // Since partial layout handles the scrollbars
    // differently, the visible lines should not
    // be taken into account. (TODO: candidate for
    // possible refactoring; should this be
    // decided here?)
    int visibleLines = 0;
    if(m_fullLayout) {
        visibleLines = numLinesOnScreen();
    }

    if(lines != verticalScrollBar()->maximum() + visibleLines) {
        QScrollBar * vsb = verticalScrollBar();
        vsb->setRange(0, lines - visibleLines);
        vsb->setPageStep(visibleLines - PAGE_STEP_OVERLAP);
        vsb->setSingleStep(1);
    }
}

void TailView::vScrollBarAction(int action)
{
    if(verticalScrollBar()->sliderPosition() > verticalScrollBar()->maximum()) {
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->maximum());
    }

    if(verticalScrollBar()->sliderPosition() < verticalScrollBar()->minimum()) {
        verticalScrollBar()->setSliderPosition(verticalScrollBar()->minimum());
    }

    if(m_fullLayout) { return; }

    int line_change = 0;
    int page_step = numLinesOnScreen() - PAGE_STEP_OVERLAP;

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

int TailView::numLinesOnScreen() const
{
    QFont font = m_document->document()->firstBlock().layout()->font();
    QFontMetrics fontMetrics(font);
    int lineHeight = fontMetrics.lineSpacing();
    int windowHeight = viewport()->height();

    int result = windowHeight / lineHeight;
    return result;
}
