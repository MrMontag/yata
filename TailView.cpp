#include "TailView.h"
#include <QTextEdit>
#include <QVBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QString>

TailView::TailView(QWidget * parent)
	: QWidget(parent)
{
	m_textView = new QTextEdit;
	m_textView->setCurrentFont(QFont("Monospace"));
	QVBoxLayout * layout = new QVBoxLayout;
	layout->addWidget(m_textView);
	setLayout(layout);
}

QString TailView::getFileContents(const QString & filename)
{
	QFile file(filename);
	if(file.open(QFile::ReadOnly | QIODevice::Text)) {
		QTextStream in(&file);
		return in.readAll();
	}
	return QString();
}

void TailView::onFileChanged(const QString & path)
{
	QFile file(path);
	if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return;
	}
	QTextStream instream(&file);
	int line_number = 0;

	const QString format = getFileContents(":/TailViewFormat.html");
	const QString line_format = getFileContents(":/TailViewLine.html");

	QString lines;
	/*content +=
		"<html><head><style type \"text/css\">"
		"body { font-family:\"Monospace\"; }"
		"span.linenumber { font-family:\"Monospace\"; background-color:grey; color:white }"
		"</style></head><body>";*/
	while(!instream.atEnd()) {
		QString file_line = instream.readLine();
		QString line = line_format;
		line.replace("__LINE_NUMBER__", QString::number(++line_number));
		line.replace("__LINE__", file_line);
		lines += line;
	}
	QString content = format;
	content.replace("__CONTENT__", lines);

	m_textView->setHtml(content);
}

