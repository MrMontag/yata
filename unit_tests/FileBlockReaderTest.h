#ifndef FILEBLOCKREADERTEST_H
#define FILEBLOCKREADERTEST_H

#include "TestObject.h"

class FileBlockReaderTest: public TestObject {
	Q_OBJECT
private slots:
	void readChunkTest_data();
	void readChunkTest();

	void sizeTest();
};
#endif
