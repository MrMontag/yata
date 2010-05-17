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
}


void TailView::paintEvent(QPaintEvent * /*event*/)
{
	QPainter painter(viewport());

	QTextLayout & layout(*m_document->firstBlock().layout());
	QFont font = layout.font();
	QFontMetrics fontMetrics(font);

    const int leading = fontMetrics.leading();

	qreal height = 0;
	qreal widthUsed = 0;
	layout.beginLayout();
    int numLines = 0;
	while(true) {
		QTextLine line = layout.createLine();
		if(!line.isValid()) break;
        numLines++;
        line.setLineWidth(viewport()->size().width());
		height += leading;
		line.setPosition(QPointF(0, height));
		height += line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
	}

    int visibleLines = numLinesOnScreen();
    if(numLines != verticalScrollBar()->maximum() + visibleLines) {
		QScrollBar * vsb = verticalScrollBar();
        vsb->setRange(0, numLines - visibleLines);
        vsb->setPageStep(visibleLines);
        vsb->setSingleStep(1);
	}

	layout.endLayout();

    layout.draw(&painter, QPoint(0, -(verticalScrollBar()->value() * fontMetrics.lineSpacing())));

} 

void TailView::vScrollBarAction(int action)
{
    qDebug("%d", action);
}

int TailView::numLinesOnScreen() const
{
    QFont font = m_document->firstBlock().layout()->font();
    QFontMetrics fontMetrics(font);
    double lineHeight = fontMetrics.lineSpacing();
    double windowHeight = viewport()->height();

    int result = std::ceil(windowHeight / lineHeight);
    qDebug("%s: %d float: %f", __FUNCTION__, result, windowHeight / lineHeight);
    return result;
}
