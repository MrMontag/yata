#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QWidget>

class QTextEdit;
class QString;

class TailView: public QWidget {
	Q_OBJECT
public:
	TailView(QWidget * parent);
public slots:
	void onFileChanged(const QString & path);
private:
	QTextEdit * m_textView;
};

#endif
