#include "TailView.h"
#include "HtmlConverter.h"
#include "FileBlockReader.h"

#include <QApplication>
#include <QDebug>
#include <QFile>
#include <QFileSystemWatcher>
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

const qint64 APPROXIMATE_CHARS_PER_LINE = 10;
const int PAGE_STEP_OVERLAP = 2;

TailView::TailView(QWidget * parent)
    : QAbstractScrollArea(parent)
    , m_document(new QTextDocument(this))
    , m_watcher(new QFileSystemWatcher(this))
    , m_fileChanged(false)
    , m_lastSize(0,0)
    , m_numLayoutLines(0)
    , m_fullLayout(true)
    , m_lineOffset(0)
    , m_firstVisibleBlock(0)
    , m_firstVisibleLine(0)
    , m_lastFilePos(0)
    , m_isSearchRegex(false)
    , m_isSearchCaseSensitive(true)
    , m_textCursor(new QTextCursor())
    , m_isInDialog(false)
{
    m_document->setUndoRedoEnabled(false);
    connect(m_watcher, SIGNAL(fileChanged(const QString &)), SLOT(onFileChanged(const QString &)));
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
}

TailView::~TailView()
{
    delete m_textCursor;
}

void TailView::setFile(const QString & filename)
{
    if(!filename.isEmpty()) {
        if(m_watcher->files().size() != 0) {
            m_watcher->removePaths(m_watcher->files());
        }
    }
    m_watcher->addPath(filename);

    verticalScrollBar()->setSliderPosition(0);
    horizontalScrollBar()->setSliderPosition(0);

    onFileChanged(filename);
}

void TailView::setFullLayout(bool fullLayout)
{
    m_fullLayout = fullLayout;
    onFileChanged(m_filename);
}

bool TailView::isFullLayout() const
{
    return m_fullLayout;
}

const QString & TailView::lastSearchString() const
{
    return m_lastSearchString;
}

bool TailView::searchWasRegex() const
{
    return m_isSearchRegex;
}

bool TailView::searchWasCaseSensitive() const
{
    return m_isSearchCaseSensitive;
}

void TailView::newSearch(const QString & searchString, bool isRegex, bool caseSensitive)
{
    m_lastSearchString = searchString;
    m_isSearchRegex = isRegex;
    m_isSearchCaseSensitive = caseSensitive;
    searchForward();
}

void TailView::searchForward()
{
    search(true);
}

void TailView::searchBackward()
{
    search(false);
}

void TailView::search(bool isForward)
{
    if(m_textCursor->isNull()) {
        int topLine = verticalScrollBar()->value();
        std::vector<int>::const_iterator blockitr = std::upper_bound(
            m_layoutPositions.begin(),
            m_layoutPositions.end(),
            topLine);
        if(blockitr != m_layoutPositions.begin()) { --blockitr; }
        if(blockitr != m_layoutPositions.end()) {
            int blockNumber = blockitr - m_layoutPositions.begin();
            QTextBlock block = m_document->findBlockByNumber(blockNumber);
            int blockLine = topLine - *blockitr;
            *m_textCursor = QTextCursor(block);
            m_textCursor->movePosition(QTextCursor::Down, QTextCursor::MoveAnchor, blockLine);
        } else {
            *m_textCursor = QTextCursor(m_document);
        }
    } else {
        QTextCharFormat clear;
        m_textCursor->setCharFormat(clear);
    }

    QRegExp regex(m_lastSearchString,
        m_isSearchCaseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive,
        m_isSearchRegex ? QRegExp::RegExp2 : QRegExp::FixedString);

    QTextDocument::FindFlags flags = 0;
    if(!isForward) {
        flags |= QTextDocument::FindBackward;
    }
    QTextCursor match = m_document->find(regex, *m_textCursor, flags);

    if(match.isNull()) {
        resetSearchCursor(isForward);
        match = m_document->find(regex, *m_textCursor, flags);
    }

    if(!match.isNull()) {
        QTextCharFormat format;

        // TODO: make the palette customizable (for now use the system palette)
        QPalette palette = QApplication::palette();
        format.setBackground(palette.highlight());
        format.setForeground(palette.highlightedText());
        match.setCharFormat(format);
        *m_textCursor = match;

        scrollToIfNecessary(*m_textCursor);
    }

    viewport()->update();
}

void TailView::scrollToIfNecessary(const QTextCursor & cursor) const
{
    QTextBlock cursorBlock = cursor.block();
    int blockNumber = cursorBlock.blockNumber();
    if(blockNumber < 0 || static_cast<unsigned int>(blockNumber) >= m_layoutPositions.size()) { return; }
    int cursorLineNumber = m_layoutPositions[blockNumber];

    int blockLine = cursorBlock.layout()->lineForTextPosition(cursor.position() - cursorBlock.position()).lineNumber();

    cursorLineNumber += blockLine;

    int topScreenLine = verticalScrollBar()->value();

    int numReadableLines = numLinesOnScreen();

    if(topScreenLine <= cursorLineNumber && cursorLineNumber <= topScreenLine + numReadableLines - 1) {
        // No scrolling needed -- return
        return;
    }

    int newTopLine = cursorLineNumber - numReadableLines / 2;

    verticalScrollBar()->setValue(newTopLine);

}

void TailView::resetSearchCursor(bool isTop)
{
    *m_textCursor = QTextCursor(m_document);
    m_textCursor->movePosition(isTop ? QTextCursor::Start : QTextCursor::End);
}

void TailView::onFileChanged(const QString & path)
{
    m_filename = path;

    // TODO: Temporary stopgap until better file watching is
    // implemented -- prompt user to reload the file
    // if it gets deleted.
    bool needs_reconnect = false;

    while(!QFile::exists(path)) {
        if(m_isInDialog) { return; }
        needs_reconnect = true;
        m_isInDialog = true;
        QString msg = QString("%1 not found.  Click OK to try again.").arg(m_filename);
        int button = QMessageBox::critical(this, "yata", msg, QMessageBox::Ok, QMessageBox::Cancel);
        m_isInDialog = false;
        if(button == QMessageBox::Cancel) {
            break;
        }
    }

    if(needs_reconnect) {
        m_watcher->removePath(path);
        m_watcher->addPath(path);
    }
    // End stopgap

    m_fileChanged = true;
    if(m_fullLayout) {
        QFile file(m_filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_document->setPlainText(QString());
            viewport()->update();
            return;
        }

        QTextStream instream(&file);

        m_document->setPlainText(instream.readAll());
    } else {
        updateDocumentForPartialLayout();
    }

    viewport()->update();
}

void TailView::performLayout()
{
    bool needs_layout = !m_fullLayout || m_fileChanged || viewport()->size().rwidth() != m_lastSize.rwidth();

    if(!needs_layout) {
        return;
    }

    m_numLayoutLines = 0;
    m_layoutPositions.clear();

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        m_layoutPositions.push_back(m_numLayoutLines);
        m_numLayoutLines += layoutBlock(&block);
    }

    if(m_fullLayout) {
        updateScrollBars(m_numLayoutLines);
    }

    m_fileChanged = false;
}

int TailView::layoutBlock(QTextBlock * textBlock)
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
        line.setLineWidth(viewport()->size().width());
        height += fontMetrics.leading();
        line.setPosition(QPointF(0, height));
        height += line.height();
    }

    layout.endLayout();
    return numLines;
}

void TailView::paintEvent(QPaintEvent * /*event*/)
{
    performLayout();

    qreal dy = 0;

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        QTextLayout & layout(*block.layout());
        QFontMetrics fontMetrics(layout.font());

        qreal height = fontMetrics.lineSpacing() * layout.lineCount();

        int scrollValue = m_fullLayout ? verticalScrollBar()->sliderPosition() : m_lineOffset;
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

    m_lastSize = viewport()->size();
}

void TailView::resizeEvent(QResizeEvent *)
{
    if(m_fullLayout) {
        updateScrollBars(m_numLayoutLines);
    } else {
        updateDocumentForPartialLayout();
    }
}


void TailView::updateDocumentForPartialLayout(int line_change /* = 0 */)
{
    if(m_fullLayout) { return; }

    FileBlockReader reader(m_filename);
    int lines_on_screen = numLinesOnScreen();
    int visible_lines = lines_on_screen + 1;

    // TODO: account for wrapped lines when computing bottom_screen_pos
    qint64 bottom_screen_pos = reader.getStartPosition(reader.size(), -lines_on_screen);

    int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    updateScrollBars(approx_lines);

    qint64 file_pos = 0;
    if(0 == line_change) {
        m_lineOffset = 0; // TODO: don't reset if scrollbar didn't move
        m_firstVisibleBlock = 0;
        m_firstVisibleLine = 0;
        if(verticalScrollBar()->sliderPosition() >= verticalScrollBar()->maximum()) {
            file_pos = bottom_screen_pos;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar()->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }

        file_pos = reader.getStartPosition(file_pos, line_change);
        file_pos = std::min(file_pos, bottom_screen_pos);

        QString data;
        m_lastFilePos = reader.readChunk(&data, file_pos, 0, visible_lines).first;

        m_document->setPlainText(data);
        performLayout();

    } else {
        file_pos = m_lastFilePos;
        if(line_change < 0) {
            QString data;
            file_pos = reader.readChunk(&data, file_pos, line_change + m_firstVisibleBlock, visible_lines).first;
            m_document->setPlainText(data);
            performLayout();

            QTextBlock block = m_document->findBlockByNumber(-line_change);
            while(m_firstVisibleLine + line_change < 0) {
                line_change += m_firstVisibleLine + 1;
                block = block.previous();
                m_firstVisibleLine = block.layout()->lineCount() - 1;
            }

            m_firstVisibleBlock = block.blockNumber();
            m_firstVisibleLine += line_change;

            m_lineOffset = m_firstVisibleLine;
            for(QTextBlock block = m_document->firstBlock(); block.blockNumber() < m_firstVisibleBlock; block = block.next()) {
                m_lineOffset += block.layout()->lineCount();
            }

        } else {
            line_change += m_firstVisibleLine;
            int wrapped_line_count = 0;
            int real_line_count = 0;
            QTextBlock block = m_document->findBlockByNumber(m_firstVisibleBlock);
            while(line_change - wrapped_line_count >= block.layout()->lineCount()) {
                wrapped_line_count += block.layout()->lineCount();
                block = block.next();
                real_line_count++;
            }

            m_firstVisibleLine = line_change - wrapped_line_count;
            m_lineOffset = m_firstVisibleLine;
            m_firstVisibleBlock = 0;

            file_pos = reader.getStartPosition(file_pos, real_line_count);
            file_pos = std::min(file_pos, bottom_screen_pos);
            QString data;
            file_pos = reader.readChunk(&data, file_pos, 0, visible_lines).first;
            m_document->setPlainText(data);
            performLayout();

        }
        m_lastFilePos = file_pos;
        verticalScrollBar()->setSliderPosition(m_lastFilePos / APPROXIMATE_CHARS_PER_LINE);
    }

    viewport()->update();
}

void TailView::updateScrollBars(int lines)
{
    // Since partial layout handles the scrollbars
    // differently, the visible lines should not
    // be taken into account. (Note: candidate for
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

    updateDocumentForPartialLayout(line_change);
}

int TailView::numLinesOnScreen() const
{
    QFont font = m_document->firstBlock().layout()->font();
    QFontMetrics fontMetrics(font);
    int lineHeight = fontMetrics.lineSpacing();
    int windowHeight = viewport()->height();

    int result = windowHeight / lineHeight;
    return result;
}
