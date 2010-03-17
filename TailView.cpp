#include "TailView.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QString>
#include "HtmlConverter.h"

TailView::TailView(QWidget * parent)
	: QWidget(parent)
{
	m_textView = new QTextEdit;
	m_textView->setCurrentFont(QFont("Monospace"));
	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(m_textView);
	setLayout(layout);
}

void TailView::onFileChanged(const QString & path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QTextStream instream(&file);
	
	HtmlConverter converter;
	QString html = converter.toHtml(instream);
	m_textView->setHtml(html);
}

