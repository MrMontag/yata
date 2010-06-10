#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>

class QTextDocument;
class QString;
class QFileSystemWatcher;

class TailView: public QAbstractScrollArea {
	Q_OBJECT

public:
	TailView(QWidget * parent);
    void setFile(const QString & filename);

protected:
    void paintEvent(QPaintEvent * event);

private slots:
    void onFileChanged(const QString & path);
    void vScrollBarAction(int action);

private:
    int numLinesOnScreen() const;
    void setScrollBars(int lines);

private:
    QString m_filename;
	QTextDocument * m_document;
    QFileSystemWatcher * m_watcher;
    bool m_fileChanged;

    QSize m_lastSize;
    int m_numFileLines;
    bool m_fullLayout;
    int m_topLayoutLine;
};

#endif
