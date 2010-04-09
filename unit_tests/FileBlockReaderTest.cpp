#include "FileBlockReaderTest.h"
#include "TestRegister.h"
#include "FileBlockReader.h"
#include <QTest>

REGISTER_TEST(FileBlockReaderTest)

void FileBlockReaderTest::readChunkTest_data()
{
	QTest::addColumn<qint64>("start_pos");
	QTest::addColumn<qint64>("num_lines");
	QTest::addColumn<qint64>("res_start_pos");
	QTest::addColumn<qint64>("res_end_pos");
	QTest::addColumn<QString>("res_string");

	QTest::newRow("one line") << qint64(0) << qint64(1) << qint64(0) << qint64(15) << "This is line 1";
	QTest::newRow("2 lines") << qint64(0) << qint64(2) << qint64(0) << qint64(30) << "This is line 1\nThis is line 2";
	QTest::newRow("line 2") << qint64(15) << qint64(1) << qint64(15) << qint64(30) << "This is line 2";
	QTest::newRow("beginning of line 1") << qint64(3) << qint64(1) << qint64(0) << qint64(15) << "This is line 1";
	QTest::newRow("beginning of line 2") << qint64(17) << qint64(1) << qint64(15) << qint64(30) << "This is line 2";
}


void FileBlockReaderTest::readChunkTest()
{
	QFETCH(qint64, start_pos);
	QFETCH(qint64, num_lines);
	QFETCH(qint64, res_start_pos);
	QFETCH(qint64, res_end_pos);
	QFETCH(QString, res_string);

	FileBlockReader reader(":/unit_test/FileBlockReaderTest.txt");
	QString line;

	std::pair<qint64, qint64> result_pos = reader.readChunk(&line, start_pos, num_lines);
	QVERIFY(result_pos.first == res_start_pos);
	QVERIFY(result_pos.second == res_end_pos);
	QVERIFY(line == res_string);
}

void FileBlockReaderTest::sizeTest()
{
	FileBlockReader reader(":/unit_test/FileBlockReaderTest.txt");
	QVERIFY(reader.size() == 30);
}
