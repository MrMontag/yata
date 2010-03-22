#include <QtTest/QtTest>
#include "HtmlConverter.h"
#include "HtmlConverterTest.h"

void HtmlConverterTest::toHtmlTest_data()
{
	QTest::addColumn<QString>("input");
	QTest::addColumn<QString>("result");

	QFile format_file(":/HtmlFormat.html");

	if(!format_file.open(QFile::ReadOnly | QIODevice::Text)) {
		QFAIL("Failed to open HtmlFormat.html");
	}
	QTextStream in(&format_file);
	m_format = in.readAll();

	QTest::newRow("plain text") << "abcd" << getData("abcd");
	QTest::newRow("open angle bracket") << "<" << getData("&lt;");
	QTest::newRow("two <'s") << "<<" << getData("&lt;&lt;");
	QTest::newRow("close angle bracket") << ">" << getData("&gt;");
	QTest::newRow("two >'s") << ">>" << getData("&gt;&gt;");
	QTest::newRow("ampersand") << "&" << getData("&amp;");

}

void HtmlConverterTest::toHtmlTest()
{
	QFETCH(QString, input);
	QFETCH(QString, result);

	HtmlConverter converter;

	QTextStream stream(&input);
	QVERIFY(converter.toHtml(stream) == result);
}

QString HtmlConverterTest::getData(const QString & input)
{
	QString output = m_format;
	output.replace("__CONTENT__", input + "<br>");
	return output;
}

