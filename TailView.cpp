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

TailView::TailView(QWidget * parent)
	: QAbstractScrollArea(parent)
	, m_document(new QTextDocument(this))
    , m_watcher(new QFileSystemWatcher(this))
    , m_fileChanged(false)
    , m_lastSize(0,0)
    , m_numFileLines(0)
    , m_fullLayout(true)
    , m_topLayoutLine(0)
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

    onFileChanged(filename);
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
    }
    verticalScrollBar()->setValue(0);
    horizontalScrollBar()->setValue(0);
    m_fileChanged = true;
    viewport()->update();
}


void TailView::paintEvent(QPaintEvent * /*event*/)
{
    if(!m_fullLayout) {
        FileBlockReader reader(m_filename);
        int approx_lines = static_cast<int>(reader.size() / APPROXIMATE_CHARS_PER_LINE);
        setScrollBars(approx_lines);

        qint64 file_pos = static_cast<qint64>(verticalScrollBar()->value()) * APPROXIMATE_CHARS_PER_LINE;

        QString data;
        int visible_lines = numLinesOnScreen();
        reader.readChunk(&data, file_pos, 0, visible_lines);
        QTextStream textStream(&data);
        HtmlConverter converter;
        QString html = converter.toHtml(textStream);
        m_document->setHtml(html);
    }

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

        QPoint start(0, (m_fullLayout ? -(verticalScrollBar()->value() * fontMetrics.lineSpacing()) + dy : dy));
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
        setScrollBars(m_numFileLines);
    }

    m_fileChanged = false;
    m_lastSize = viewport()->size();
}

void TailView::setScrollBars(int lines)
{
    int visibleLines = numLinesOnScreen();
    if(lines != verticalScrollBar()->maximum() + visibleLines) {
        QScrollBar * vsb = verticalScrollBar();
        vsb->setRange(0, lines - visibleLines);
        vsb->setPageStep(visibleLines);
        vsb->setSingleStep(1);
    }
}

void TailView::vScrollBarAction(int action)
{
    if(m_fullLayout) { return; }

    switch(action) {
    case QAbstractSlider::SliderSingleStepAdd:
        break;
    case QAbstractSlider::SliderSingleStepSub:
        break;
    default:
        break;
    }
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
