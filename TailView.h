#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>
#include <QScopedPointer>
#include <vector>

class QTextDocument;
class QString;
class QTextBlock;
class QTextCursor;
class DocumentSearch;
class YFileSystemWatcherThread;

class TailView: public QAbstractScrollArea {
    Q_OBJECT

public:
    TailView(QWidget * parent);
    ~TailView();
    void setFile(const QString & filename);

    void setFullLayout(bool fullLayout);
    bool isFullLayout() const;

    const QString & lastSearchString() const;
    bool searchWasRegex() const;
    bool searchWasCaseSensitive() const;

public slots:
    void newSearch(const QString & searchString, bool isRegex, bool caseSensitive);
    void searchForward();
    void searchBackward();

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent *);

private slots:
    void onFileChanged();
    void onFileDeleted();
    void vScrollBarAction(int action);

private:
    int numLinesOnScreen() const;
    void setDocumentText(const QString & data);
    void updateScrollBars(int lines);
    void updateDocumentForPartialLayout(int line_change = 0);
    void performLayout();
    int layoutBlock(QTextBlock * textBlock);

    void searchFile(bool isForward);
    bool searchDocument(bool isForward, bool wrapAround = true);
    void scrollToIfNecessary(const QTextCursor & cursor);

private:
    QString m_filename;
    QTextDocument * m_document;
    QScopedPointer<YFileSystemWatcherThread> m_watcher;
    bool m_fileChanged;

    QSize m_lastSize;
    int m_numLayoutLines;
    std::vector<int> m_layoutPositions;
    std::vector<qint64> m_filePositions;

    bool m_fullLayout;

    int m_lineOffset;
    int m_firstVisibleBlock;
    int m_firstVisibleLine;
    qint64 m_lastFilePos;

    QScopedPointer<DocumentSearch> m_documentSearch;
    qint64 m_cursorLinePos;
    int m_cursorLineOffset;
    int m_cursorLength;

};

#endif
