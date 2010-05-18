#include "TailView.h"
#include "HtmlConverter.h"

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

TailView::TailView(QWidget * parent)
	: QAbstractScrollArea(parent)
	, m_document(new QTextDocument(this))
    , m_fileChanged(false)
    , m_lastSize(0,0)
    , m_numFileLines(0)
{
    connect(verticalScrollBar(), SIGNAL(actionTriggered(int)), SLOT(vScrollBarAction(int)));
}

void TailView::onFileChanged(const QString & path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		m_document->setHtml(QString());
		return;
	}
	QTextStream instream(&file);
	
	HtmlConverter converter;
	QString html = converter.toHtml(instream);
    m_document->setHtml(html);
    viewport()->update();
    m_fileChanged = true;
}


void TailView::paintEvent(QPaintEvent * /*event*/)
{
	QPainter painter(viewport());

	QTextLayout & layout(*m_document->firstBlock().layout());
	QFont font = layout.font();
	QFontMetrics fontMetrics(font);

    const int leading = fontMetrics.leading();

    if(m_fileChanged || viewport()->size().rwidth() != m_lastSize.rwidth()) {
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

    int visibleLines = numLinesOnScreen();
    if(m_numFileLines != verticalScrollBar()->maximum() + visibleLines) {
        QScrollBar * vsb = verticalScrollBar();
        vsb->setRange(0, m_numFileLines - visibleLines);
        vsb->setPageStep(visibleLines);
        vsb->setSingleStep(1);
    }

    layout.draw(
        &painter,
        QPoint(0, -(verticalScrollBar()->value() * fontMetrics.lineSpacing())),
        QVector<QTextLayout::FormatRange>(),
        QRectF(QPointF(0,0), viewport()->size()));
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
