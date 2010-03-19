#ifndef HTMLCONVERTER_H
#define HTMLCONVERTER_H

class QString;
class QTextStream;

class HtmlConverter {
public:
	QString toHtml(QTextStream & stream);
private:
	QString getFileContents(const QString & filename);
	QString convertLine(const QString & s);
};

#endif
