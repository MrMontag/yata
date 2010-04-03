#ifndef TAIL_VIEW_TEST_H
#define TAIL_VIEW_TEST_H

#include "TestObject.h"

class TailViewTest: public TestObject {
	Q_OBJECT
signals:
	void fileChangedTestSignal(const QString &);
private slots:
	void onFileChangedTest();
	
};

#endif
