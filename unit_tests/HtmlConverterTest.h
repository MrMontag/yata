#ifndef HTML_CONVERTER_TEST_H
#define HTML_CONVERTER_TEST_H

#include <QObject>
#include <QString>

class HtmlConverterTest: public QObject {
	Q_OBJECT
private slots:
	void toHtmlTest_data();
	void toHtmlTest();
private:
	QString getData(const QString & input);
private:
	QString m_format;
};

#endif
