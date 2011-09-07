/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "TailView.h"
#include "DocumentSearch.h"
#include "FileBlockReader.h"
#include "FileSearch.h"
#include "FullLayout.h"
#include "PartialLayout.h"
#include "preferences/Preferences.h"
#include "SearchInfo.h"
#include "YApplication.h"
#include "YFileCursor.h"
#include "YFileSystemWatcherThread.h"
#include "YTextDocument.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QString>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextStream>

#include <algorithm>

const qint64 MAX_FULL_LAYOUT_FILE_SIZE = 1024 * 64;

TailView::TailView(QWidget * parent)
    : QAbstractScrollArea(parent)
    , m_document(new YTextDocument)
    , m_isActive(false)
    , m_layoutType(AutomaticLayout)
    , m_fullLayoutStrategy(new FullLayout(this))
    , m_partialLayoutStrategy(new PartialLayout(this))
    , m_layoutStrategy(m_partialLayoutStrategy.data())
    , m_followTail(true)
    , m_documentSearch(new DocumentSearch(m_document->document()))
{
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
    connect(Preferences::instance(), SIGNAL(preferencesChanged()), SLOT(onPreferencesChanged()));
    m_document->setFont(Preferences::instance()->font());
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

void TailView::newSearch()
{
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
    const SearchCriteria & sc = SearchInfo::instance().search();
    m_documentSearch->setSearchCriteria(sc.expression, sc.isRegex, sc.isCaseSensitive);
    bool wrapAround = m_layoutStrategy->wrapAroundForDocumentSearch();
    bool matchFound = searchDocument(isForward, wrapAround);
    if(matchFound) {
        scrollToIfNecessary(documentSearch()->cursor());
    } else {
        matchFound = m_layoutStrategy->searchFile(isForward);
    }

    if(!matchFound) {
        QString message;
        QTextStream(&message)
            << QObject::tr("Pattern \"")
            << documentSearch()->lastSearchString()
            << QObject::tr("\" not found");
        QMessageBox::information(this, YApplication::displayAppName(), message);
    }

    viewport()->update();
}

bool TailView::searchDocument(bool isForward, bool wrapAround)
{
    if(!m_document->fileCursor().isNull()) {
        m_documentSearch->setCursor(m_document->fileCursor().qTextCursor(m_document.data()));
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
        int cursorBeginPos = std::min(searchCursor.position(), searchCursor.anchor());
        YFileCursor newCursor(m_document->lineAddresses()[blockNum], 
            cursorBeginPos - searchCursor.block().position(),
            std::abs(searchCursor.anchor() - searchCursor.position()));
        m_document->setFileCursor(newCursor);
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

void TailView::setActive(bool active)
{
    // No need to perform any action if we're not changing state
    if (active == m_isActive) { return; }

    m_isActive = active;

    SearchInfo & si = SearchInfo::instance();

    if (active) {
        connect(&si, SIGNAL(newSearch()), SLOT(newSearch()));
    } else {
        disconnect(&si, SIGNAL(newSearch()), this, SLOT(newSearch()));
    }
}

void TailView::onFileChanged()
{
    bool fullLayout = false;
    switch(m_layoutType) {
    case DebugFullLayout: fullLayout = true; break;
    case DebugPartialLayout: fullLayout = false; break;
    case AutomaticLayout:
        fullLayout = (QFileInfo(m_filename).size() <= MAX_FULL_LAYOUT_FILE_SIZE);
        break;
    }

    if(fullLayout) {
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
    onFileChanged();
}

void TailView::paintEvent(QPaintEvent * /*event*/)
{
    m_layoutStrategy->performLayout();

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

void TailView::updateScrollBars(int newMax)
{
    if(verticalScrollBar()->maximum() != newMax) {
        QScrollBar * vsb = verticalScrollBar();
        vsb->setRange(0, newMax);
        vsb->setPageStep(numLinesOnScreen() - PAGE_STEP_OVERLAP);
        vsb->setSingleStep(1);
    }
}

void TailView::vScrollBarAction(int action)
{
    QScrollBar * vsb = verticalScrollBar();
    if(vsb->sliderPosition() > vsb->maximum()) {
        vsb->setSliderPosition(vsb->maximum());
    }

    if(vsb->sliderPosition() < vsb->minimum()) {
        vsb->setSliderPosition(vsb->minimum());
    }

    m_layoutStrategy->vScrollBarAction(action);
}

void TailView::onPreferencesChanged()
{
    m_document->setFont(Preferences::instance()->font());
    onFileChanged();
}

int TailView::numLinesOnScreen() const
{
    QFont font = m_document->document()->defaultFont();
    QFontMetrics fontMetrics(font);
    int lineHeight = fontMetrics.lineSpacing();
    int windowHeight = viewport()->height();

    int result = windowHeight / lineHeight;
    return result;
}
