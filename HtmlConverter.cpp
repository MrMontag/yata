#include "HtmlConverter.h"
#include <QString>
#include <QTextStream>
#include <QFile>

QString HtmlConverter::getFileContents(const QString & filename)
{
	QFile file(filename);
	if(file.open(QFile::ReadOnly | QIODevice::Text)) {
		QTextStream in(&file);
		return in.readAll();
	}
	return QString();
}

QString HtmlConverter::convertLine(const QString & s)
{
	QString new_str(s);
	new_str.replace('&', "&amp;"); // Needs to be before others
	new_str.replace('<', "&lt;");
	new_str.replace('>', "&gt;");
	return new_str;
}

QString HtmlConverter::toHtml(QTextStream & instream)
{
	const QString format = getFileContents(":/HtmlFormat.html");

	QString lines;
	while(!instream.atEnd()) {
		lines += convertLine(instream.readLine()) + "<br>";
	}
	QString content = format;
	content.replace("__CONTENT__", lines);
	return content;
}
