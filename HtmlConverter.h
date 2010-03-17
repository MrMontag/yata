#ifndef HTMLCONVERTER_H
#define HTMLCONVERTER_H

class QString;
class QTextStream;

class HtmlConverter {
public:
	QString getFileContents(const QString & filename);
	QString convertLine(const QString & s);
	QString toHtml(QTextStream & stream);
};

#endif
