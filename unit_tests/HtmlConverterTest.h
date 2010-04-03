#ifndef HTML_CONVERTER_TEST_H
#define HTML_CONVERTER_TEST_H

#include "TestObject.h"
#include <QString>

class HtmlConverterTest: public TestObject {
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
