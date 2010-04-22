#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>

class QTextDocument;
class QString;

class TailView: public QAbstractScrollArea {
	Q_OBJECT
public:
	TailView(QWidget * parent);
public slots:
	void onFileChanged(const QString & path);
protected:
	void paintEvent(QPaintEvent * event); 
private:
	QTextDocument * m_document;
};

#endif
