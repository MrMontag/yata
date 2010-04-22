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

TailView::TailView(QWidget * parent)
	: QAbstractScrollArea(parent)
	, m_document(new QTextDocument(this))
{
	
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
	static int i = 0;
	qDebug() << __FUNCTION__ << i++; 
	QPainter painter(viewport());

	QTextLayout & layout(*m_document->firstBlock().layout());
	QFont font = layout.font();
	QFontMetrics fontMetrics(font);

	int leading = fontMetrics.leading();

	qreal height = 0;
	qreal widthUsed = 0;
	layout.beginLayout();
	int singleStep = 1;
	while(true) {
		QTextLine line = layout.createLine();
		if(!line.isValid()) break;
		line.setLineWidth(viewport()->size().width());
		height += leading;
		line.setPosition(QPointF(0, height));
		height += line.height();
		singleStep = line.height();
		widthUsed = qMax(widthUsed, line.naturalTextWidth());
	}

	if(height != verticalScrollBar()->maximum() + viewport()->height()) {
		QScrollBar * vsb = verticalScrollBar();
		vsb->setRange(0, height - viewport()->height());
		vsb->setPageStep(viewport()->height());
		vsb->setSingleStep(singleStep);
	}

	layout.endLayout();

	layout.draw(&painter, QPoint(0, -verticalScrollBar()->value()));

} 
