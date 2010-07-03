#include "TailView.h"
#include "HtmlConverter.h"
#include "FileBlockReader.h"

#include <QDebug>
#include <QFile>
#include <QFileSystemWatcher>
#include <QPainter>
#include <QScrollBar>
#include <QString>
#include <QTextBlock>
#include <QTextCursor>
#include <QTextDocument>
#include <QTextLayout>
#include <QTextStream>

#include <cmath>

const qint64 APPROXIMATE_CHARS_PER_LINE = 10;
const int PAGE_STEP_OVERLAP = 2;

TailView::TailView(QWidget * parent)
    : QAbstractScrollArea(parent)
    , m_document(new QTextDocument(this))
    , m_watcher(new QFileSystemWatcher(this))
    , m_fileChanged(false)
    , m_lastSize(0,0)
    , m_numFileLines(0)
    , m_fullLayout(true)
    , m_topLayoutLine(0)
    , m_lastFilePos(0)
{
    m_document->setUndoRedoEnabled(false);
    connect(m_watcher, SIGNAL(fileChanged(const QString &)), SLOT(onFileChanged(const QString &)));
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
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

void TailView::onFileChanged(const QString & path)
{
    m_filename = path;
    if(m_fullLayout) {
        QFile file(m_filename);
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            m_document->setPlainText(QString());
            return;
        }
        QTextStream instream(&file);

        m_document->setPlainText(instream.readAll());
    } else {
        updateDocumentForPartialLayout();
    }

    m_fileChanged = true;
    viewport()->update();
}

void TailView::performLayout()
{
    bool needs_layout = !m_fullLayout || m_fileChanged || viewport()->size().rwidth() != m_lastSize.rwidth();

    if(!needs_layout) {
        return;
    }

    m_numFileLines = 0;

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        m_numFileLines += layoutBlock(&block);
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

        int scrollValue = m_fullLayout ? verticalScrollBar()->sliderPosition() : m_topLayoutLine;
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

    if(m_fullLayout) {
        updateScrollBars(m_numFileLines);
    }

    m_lastSize = viewport()->size();
}

void TailView::resizeEvent(QResizeEvent *)
{
    if(!m_fullLayout) {
        updateDocumentForPartialLayout();
    }
}


void TailView::updateDocumentForPartialLayout(int line_change /* = 0 */)
{
    if(m_fullLayout) { return; }

    FileBlockReader reader(m_filename);
    int lines_on_screen = numLinesOnScreen();
    int visible_lines = lines_on_screen + 1;

    qint64 bottom_screen_pos = reader.getStartPosition(reader.size(), -lines_on_screen);

    int approx_lines = static_cast<int>(bottom_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    updateScrollBars(approx_lines);

    qint64 file_pos = 0;
    if(0 == line_change) {
        m_topLayoutLine = 0; // TODO: don't reset if scrollbar didn't move
        if(verticalScrollBar()->sliderPosition() >= verticalScrollBar()->maximum()) {
            file_pos = bottom_screen_pos;
        } else {
            file_pos = static_cast<qint64>(verticalScrollBar()->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
        }
        QString data;
        m_lastFilePos = reader.readChunk(&data, file_pos, line_change, visible_lines).first;

        if(m_lastFilePos > bottom_screen_pos) {
            m_lastFilePos = reader.readChunk(&data, bottom_screen_pos, 0, visible_lines).first;
        }

        m_document->setPlainText(data);

    } else {
        file_pos = m_lastFilePos;
        if(line_change < 0) {
            while(line_change + m_topLayoutLine < 0 && file_pos > 0) {
                line_change += m_topLayoutLine;
                QString line;
                file_pos = reader.readChunk(&line, file_pos, -1, 1).first;
                QTextCursor cursor(m_document);
                cursor.insertText(line);
                QTextBlock block = m_document->firstBlock();
                layoutBlock(&block);
                m_topLayoutLine = block.layout()->lineCount();
                cursor.movePosition(QTextCursor::End);
                cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
            }
            m_topLayoutLine += line_change;
            if(m_topLayoutLine < 0) {
                m_topLayoutLine = 0;
            }
        } else {
            line_change += m_topLayoutLine;
            qint64 bottom_of_screen_pos = reader.getStartPosition(file_pos, visible_lines - 1);
            while(bottom_of_screen_pos < reader.size()) {
                QTextBlock block = m_document->firstBlock();
                int line_count = block.layout()->lineCount();
                if(line_change < line_count) {
                    break;
                }
                line_change -= line_count;
                QString line;
                bottom_of_screen_pos = reader.readChunk(&line, bottom_of_screen_pos, 1, 1).first;
                QTextCursor cursor(m_document);
                cursor.movePosition(QTextCursor::End);
                cursor.insertText(line);
                block = m_document->lastBlock();
                layoutBlock(&block);
                cursor.movePosition(QTextCursor::Start);
                cursor.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
                cursor.removeSelectedText();
                file_pos = reader.getStartPosition(file_pos, 1);
            }
            m_topLayoutLine = line_change;
        }
        m_lastFilePos = file_pos;
        verticalScrollBar()->setSliderPosition(m_lastFilePos / APPROXIMATE_CHARS_PER_LINE);
    }

    performLayout();

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
