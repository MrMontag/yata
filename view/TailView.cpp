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
#include "preferences/TextColor.h"
#include "SearchInfo.h"
#include "YApplication.h"
#include "YFileCursor.h"
#include "YFileSystemWatcherThread.h"
#include "document/YTextDocument.h"
#include "document/BlockDataVector.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDir>
#include <QKeySequence>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QScrollBar>
#include <QStringBuilder>
#include <QTextLayout>
#include <QTextStream>

const qint64 MAX_FULL_LAYOUT_FILE_SIZE = 1024 * 64;

TailView::TailView(QWidget * parent)
    : QAbstractScrollArea(parent)
    , m_document(new YTextDocument)
    , m_isActive(false)
    , m_leftMouseIsDown(false)
    , m_layoutType(AutomaticLayout)
    , m_fullLayoutStrategy(new FullLayout(this))
    , m_partialLayoutStrategy(new PartialLayout(this))
    , m_layoutStrategy(m_partialLayoutStrategy.data())
    , m_followTail(true)
    , m_documentSearch(new DocumentSearch(m_document.data()))
{
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
    connect(Preferences::instance(), SIGNAL(preferencesChanged()), SLOT(onPreferencesChanged()));

    QAction * copy = new QAction(tr("&Copy"), this);
    copy->setShortcut(QKeySequence(QKeySequence::Copy));
    connect(copy, SIGNAL(triggered()), SLOT(onCopy()));
    addAction(copy);

    QAction * separator = new QAction(this);
    separator->setSeparator(true);
    addAction(separator);

    QAction * selectAll = new QAction(tr("Select &All"), this);
    selectAll->setShortcut(QKeySequence(QKeySequence::SelectAll));
    addAction(selectAll);
    setContextMenuPolicy(Qt::ActionsContextMenu);
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
        m_document->clearSelection();
    }

    viewport()->update();
}

QPoint TailView::docGraphicalPosition(const QPoint & viewPoint)
{
    int lineSpacing = QFontMetrics(m_document->font()).lineSpacing();
    double docTop = m_layoutStrategy->topScreenLine() * lineSpacing;
    double doc_y = viewPoint.y() + docTop;
    return QPoint(viewPoint.x(), doc_y);
}

bool TailView::searchDocument(bool isForward, bool wrapAround)
{
    if(!m_document->fileCursor().isNull()) {
        m_documentSearch->setCursor(m_document->fileCursor());
    } else {
        const int topLine = verticalScrollBar()->value();
        int layoutLine = 0;
        QTextBlock block = m_document->blockLayoutLines().findContainingBlock(topLine, &layoutLine);
        if(block.isValid()) {
            int blockLine = topLine - layoutLine;
            QTextCursor cursor(block);
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, blockLine);
            YFileCursor ycursor = m_document->yFileCursor(cursor);
            m_documentSearch->setCursor(ycursor);
        } else {
            m_documentSearch->setCursor(YFileCursor());
        }
    }

    bool foundMatch = m_documentSearch->searchDocument(isForward, wrapAround);

    if(foundMatch) {
        const YFileCursor & searchCursor = m_documentSearch->cursor();
        m_document->select(searchCursor);
    }

    return foundMatch;
}

void TailView::scrollToIfNecessary(const YFileCursor & ycursor)
{
    QTextBlock cursorBlock = m_document->lineAddresses().findContainingBlock(ycursor.lineAddress());
    int cursorLineNumber = m_document->blockLayoutLines().at(cursorBlock.blockNumber());
    int blockLine = cursorBlock.layout()->lineForTextPosition(ycursor.charPos()).lineNumber();
    cursorLineNumber += blockLine;

    int topScreenLine = m_layoutStrategy->topScreenLine();
    int numReadableLines = numLinesOnScreen();

    if(topScreenLine <= cursorLineNumber && cursorLineNumber < topScreenLine + numReadableLines) {
        // No scrolling needed -- return
        return;
    }

    int newTopLine = cursorLineNumber - numReadableLines / 2;
    m_layoutStrategy->scrollTo(newTopLine);
}

void TailView::onCopy(bool x11Selection)
{
    const YFileCursor & cursor = m_document->fileCursor();
    if(cursor.length() == 0) {
        return;
    }

    qint64 startAddress = cursor.charAddress();
    qint64 length = cursor.length();
    if(length < 0) {
        length = -length;
        startAddress -= length;
    }

    FileBlockReader reader(m_filename);
    QString data;
    if(reader.readChunk(&data, startAddress, length)) {
        QApplication::clipboard()->setText(data, x11Selection ? QClipboard::Selection: QClipboard::Clipboard);
    }
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

    QString error;
    if(m_layoutStrategy->onFileChanged(&error)) {
        if(!m_currentFileError.isEmpty()) {
            m_currentFileError.clear();
            emit fileErrorCleared();
        }
    } else {
        m_currentFileError = tr("ERROR - ") % QDir::toNativeSeparators(m_filename) % ": " % error;
        emit fileError(m_currentFileError);
    }

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

void TailView::mousePressEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftMouseIsDown = true;
        QPoint docPos(docGraphicalPosition(event->pos()));
        if(event->modifiers() == Qt::ShiftModifier) {
            m_document->moveSelect(docPos);
        } else if (event->modifiers() == 0) {
            m_document->startSelect(docPos);
        }
        viewport()->update();
    }
}

void TailView::mouseReleaseEvent(QMouseEvent * event)
{
    if(event->button() == Qt::LeftButton) {
        m_leftMouseIsDown = false;
        mouseMoveEvent(event);
        onCopy(true);
    }
}

#include <QtDebug>
void TailView::mouseMoveEvent(QMouseEvent * event)
{
    if(m_leftMouseIsDown) {
        QPoint docPos(docGraphicalPosition(event->pos()));
        m_document->moveSelect(docPos);
        viewport()->update();
    }
}

void TailView::mouseDoubleClickEvent(QMouseEvent * /*event*/)
{
}

void TailView::paintEvent(QPaintEvent * event)
{
    m_layoutStrategy->performLayout();

    QPainter painter(viewport());
    QRectF viewrect(event->rect());
    painter.fillRect(viewrect, Preferences::instance()->normalTextColor().background());

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        const QTextLayout * layout = block.layout();
        QFontMetrics fontMetrics(layout->font());

        qreal dy = m_document->blockGraphicalPositions().at(block.blockNumber());

        int scrollValue = m_layoutStrategy->topScreenLine();
        QPoint start(0, dy - scrollValue * fontMetrics.lineSpacing());
        QRectF layoutRect(layout->boundingRect());
        layoutRect.moveTo(start);
        if(viewrect.intersects(layoutRect)) {
            layout->draw(&painter, start);
        }
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

void TailView::wheelEvent(QWheelEvent * event)
{
    if(event->orientation() == Qt::Horizontal) {
        QAbstractScrollArea::wheelEvent(event);
        return;
    }

    const int SCROLL_STEP_SIZE = 120;
    int scrollSteps = event->delta() / SCROLL_STEP_SIZE;
    int linesToScroll = -(scrollSteps * qApp->wheelScrollLines());
    m_layoutStrategy->scrollBy(linesToScroll);
    event->accept();
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

    m_layoutStrategy->vScrollBarAction(action);
}

void TailView::onPreferencesChanged()
{
    m_document->markDirty();
}

int TailView::numLinesOnScreen() const
{
    const QFont & font = Preferences::instance()->font();
    QFontMetrics fontMetrics(font);
    int lineHeight = fontMetrics.lineSpacing();
    int windowHeight = viewport()->height();

    int result = windowHeight / lineHeight;
    return result;
}
