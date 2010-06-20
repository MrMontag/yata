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
            m_document->setHtml(QString());
            return;
        }
        QTextStream instream(&file);

        HtmlConverter converter;
        QString html = converter.toHtml(instream);
        m_document->setHtml(html);
    } else {
        updateDocumentForPartialLayout();
    }

    m_fileChanged = true;
    viewport()->update();
}


void TailView::paintEvent(QPaintEvent * /*event*/)
{
    bool needs_layout = !m_fullLayout || m_fileChanged || viewport()->size().rwidth() != m_lastSize.rwidth();

    if(needs_layout) {
        m_numFileLines = 0;
    }
    qreal widthUsed = 0;
    qreal dy = 0;

    for(QTextBlock block = m_document->begin(); block != m_document->end(); block = block.next()) {
        QTextLayout & layout(*block.layout());
        QFontMetrics fontMetrics(layout.font());

        qreal height = 0;

        if(needs_layout) {
           layout.beginLayout();
           while(true) {
               QTextLine line = layout.createLine();
               if(!line.isValid()) { break; }
               m_numFileLines++;
               line.setLineWidth(viewport()->size().width());
               height += fontMetrics.leading();
               line.setPosition(QPointF(0, height));
               height += line.height();
               widthUsed = qMax(widthUsed, line.naturalTextWidth());
           }

           layout.endLayout();
        } else {
            height += fontMetrics.lineSpacing() * layout.lineCount();
        }

        QPoint start(0, (m_fullLayout ? -(verticalScrollBar()->sliderPosition() * fontMetrics.lineSpacing()) + dy : dy));
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

    m_fileChanged = false;
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
    QString temp;
    int lines_on_screen = numLinesOnScreen();

    qint64 last_screen_pos = reader.readChunk(&temp, reader.size(), -lines_on_screen, 0).first;

    int approx_lines = static_cast<int>(last_screen_pos / APPROXIMATE_CHARS_PER_LINE);
    updateScrollBars(approx_lines);

    qint64 file_pos = m_lastFilePos;
    if(verticalScrollBar()->sliderPosition() == verticalScrollBar()->maximum()) {
        file_pos = last_screen_pos;
    } else if(0 == line_change) {
        file_pos = static_cast<qint64>(verticalScrollBar()->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
    }

    QString data;
    int visible_lines = lines_on_screen + 1;
    m_lastFilePos = reader.readChunk(&data, file_pos, line_change, visible_lines).first;

    if(line_change != 0) {
        verticalScrollBar()->setSliderPosition(m_lastFilePos / APPROXIMATE_CHARS_PER_LINE);
    }

    QTextStream textStream(&data);
    HtmlConverter converter;
    QString html = converter.toHtml(textStream);
    m_document->setHtml(html);

}

void TailView::updateScrollBars(int lines)
{
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
