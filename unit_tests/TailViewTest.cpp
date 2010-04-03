#include <QtTest/QtTest>
#include "TailView.h"
#include "TailViewTest.h"
#include "TestRegister.h"
#include <QTextEdit>

REGISTER_TEST(TailViewTest)

void TailViewTest::onFileChangedTest()
{
	TailView tailView(0);

	connect(this, SIGNAL(fileChangedTestSignal(const QString &)), &tailView, SLOT(onFileChanged(const QString &)));

	// In order to avoid making TailView::m_textView a public
	// variable, use Qt's introspection to get the TextEdit
	// variable needed for this test.
	QTextEdit * textEdit = tailView.findChild<QTextEdit*>();
	QVERIFY(textEdit);

	QString filename(":/unit_test/TailViewTestData.txt");
	QFile file(filename);
	file.open(QFile::ReadOnly | QIODevice::Text);
	QTextStream filestream(&file);
	QString data(filestream.readAll());

	QVERIFY(!data.isEmpty());

	emit fileChangedTestSignal(":/unit_test/TailViewTestData.txt");
	QVERIFY(textEdit->toPlainText() == data);

	emit fileChangedTestSignal(":/unit_test/does not exist");
	QVERIFY(textEdit->toPlainText() == QString());
}

