#ifndef TAIL_VIEW_TEST_H
#define TAIL_VIEW_TEST_H

#include <QObject>

class TailViewTest: public QObject {
	Q_OBJECT
signals:
	void fileChangedTestSignal(const QString &);
private slots:
	void onFileChangedTest();
	
};

#endif
