#include "TailView.h"
#include "HtmlConverter.h"
#include "FileBlockReader.h"

#include <QDebug>
#include <QFile>
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
    , m_fileChanged(false)
    , m_lastSize(0,0)
    , m_numFileLines(0)
    , m_fullLayout(false)
{
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
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
        reader.readChunk(&data, file_pos, visible_lines);
        QTextStream textStream(&data);
        HtmlConverter converter;
        QString html = converter.toHtml(textStream);
        m_document->setHtml(html);
    }

    QTextLayout & layout(*m_document->firstBlock().layout());
    QFont font = layout.font();
    QFontMetrics fontMetrics(font);

    const int leading = fontMetrics.leading();

    if(!m_fullLayout || m_fileChanged || viewport()->size().rwidth() != m_lastSize.rwidth()) {
       qreal height = 0;
       qreal widthUsed = 0;
       layout.beginLayout();
       m_numFileLines = 0;
       while(true) {
           QTextLine line = layout.createLine();
           if(!line.isValid()) break;
           m_numFileLines++;
           line.setLineWidth(viewport()->size().width());
           height += leading;
           line.setPosition(QPointF(0, height));
           height += line.height();
           widthUsed = qMax(widthUsed, line.naturalTextWidth());
       }

       layout.endLayout();
    }

    m_fileChanged = false;
    m_lastSize = viewport()->size();

    if(m_fullLayout) {
        setScrollBars(m_numFileLines);
    }

    QPainter painter(viewport());
    QPoint start(0, (m_fullLayout ? -(verticalScrollBar()->value() * fontMetrics.lineSpacing()) : 0));
    layout.draw(
        &painter,
        start,
        QVector<QTextLayout::FormatRange>(),
        QRectF(QPointF(0,0), viewport()->size()));
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

void TailView::vScrollBarAction(int /*action*/)
{
}

int TailView::numLinesOnScreen() const
{
    QFont font = m_document->firstBlock().layout()->font();
    QFontMetrics fontMetrics(font);
    double lineHeight = fontMetrics.lineSpacing();
    double windowHeight = viewport()->height();

    int result = std::ceil(windowHeight / lineHeight);
    return result;
}
