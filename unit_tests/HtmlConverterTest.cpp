#include <QtTest/QtTest>
#include "HtmlConverter.h"

class HtmlConverterTest: public QObject {
	Q_OBJECT
private slots:
	void convertLineTest_data();
	void convertLineTest();

	void toHtmlTest_data();
	void toHtmlTest();
private:
	QString getData(const QString & input);
private:
	QString m_format;
};

void HtmlConverterTest::convertLineTest_data()
{
	QTest::addColumn<QString>("str");
	QTest::addColumn<QString>("result");

	QTest::newRow("plain text") << "abcd" << "abcd";
	QTest::newRow("open angle bracket") << "<" << "&lt;";
	QTest::newRow("two <'s") << "<<" << "&lt;&lt;";
	QTest::newRow("close angle bracket") << ">" << "&gt;";
	QTest::newRow("two >'s") << ">>" << "&gt;&gt;";
	QTest::newRow("ampersand") << "&" << "&amp;";
}

void HtmlConverterTest::convertLineTest()
{
	QFETCH(QString, str);
	QFETCH(QString, result);

	HtmlConverter converter;
	
	QVERIFY(converter.convertLine(str) == result);
}

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

	QTest::newRow("asdf") << "asdf" << getData("asdf");
	QTest::newRow("<") << "<" << getData("&lt;");
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

QTEST_MAIN(HtmlConverterTest)
#include "HtmlConverterTest.moc"
