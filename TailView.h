#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>
#include <QScopedPointer>
#include <vector>

class QString;
class QTextBlock;
class QTextCursor;
class DocumentSearch;
class FileBlockReader;
class YFileCursor;
class YFileSystemWatcherThread;
class YTextDocument;

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
    void onFileChanged();

protected:
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent * event);

private slots:
    void onFileDeleted();
    void vScrollBarAction(int action);

private:
    int numLinesOnScreen() const;
    void setDocumentText(const QString & data);
    void updateScrollBars(int lines);
    void updateDocumentForPartialLayout(int line_change = 0, qint64 new_line_address = -1);
    void performLayout();

    void searchFile(bool isForward);
    bool searchDocument(bool isForward, bool wrapAround = true);
    void scrollToIfNecessary(const QTextCursor & cursor);

    // TODO: move to YTextCursor
    QTextCursor qTextCursor(const YFileCursor & fileCursor);
private:
    QString m_filename;
    QScopedPointer<YTextDocument> m_document;
    QScopedPointer<YFileSystemWatcherThread> m_watcher;

    // TODO: YTextDocument should own m_lineAddresses
    std::vector<qint64> m_lineAddresses;

    bool m_fullLayout;

    int m_firstVisibleLayoutLine;
    int m_firstVisibleBlock;
    int m_firstVisibleBlockLine;
    qint64 m_lastFileAddress;

    QScopedPointer<DocumentSearch> m_documentSearch;

    // TODO: YTextDocument should own m_fileCursor
    QScopedPointer<YFileCursor> m_fileCursor;
    QScopedPointer<FileBlockReader> m_blockReader;
};

#endif
