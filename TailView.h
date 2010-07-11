#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>

class QTextDocument;
class QString;
class QFileSystemWatcher;
class QTextBlock;

class TailView: public QAbstractScrollArea {
    Q_OBJECT

public:
    TailView(QWidget * parent);
    void setFile(const QString & filename);

    void setFullLayout(bool fullLayout);
    bool isFullLayout() const;

    const QString & lastSearchString() const;
    bool searchWasRegex() const;
    bool searchWasCaseSensitive() const;

public slots:
    void search(const QString & searchString, bool isRegex, bool caseSensitive);
    void searchForward();
    void searchBackward();

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent *);

private slots:
    void onFileChanged(const QString & path);
    void vScrollBarAction(int action);

private:
    int numLinesOnScreen() const;
    void updateScrollBars(int lines);
    void updateDocumentForPartialLayout(int line_change = 0);
    void performLayout();
    int layoutBlock(QTextBlock * textBlock);

private:
    QString m_filename;
    QTextDocument * m_document;
    QFileSystemWatcher * m_watcher;
    bool m_fileChanged;

    QSize m_lastSize;
    int m_numFileLines;
    bool m_fullLayout;
    int m_lineOffset;
    int m_firstVisibleBlock;
    int m_firstVisibleLine;
    qint64 m_lastFilePos;

    QString m_lastSearchString;
    bool m_isSearchRegex;
    bool m_isSearchCaseSensitive;

    bool m_isInDialog; // ugh. Let's remove this variable as soon as we can!
};

#endif
