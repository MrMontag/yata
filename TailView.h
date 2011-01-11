/*
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
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
class LayoutStrategy;
class FullLayout;
class PartialLayout;

const int PAGE_STEP_OVERLAP = 2;

class TailView: public QAbstractScrollArea {
    Q_OBJECT

public:
    enum LayoutType {
        DebugFullLayout,
        DebugPartialLayout,
        AutomaticLayout
    };

    TailView(QWidget * parent);
    ~TailView();
    void setFile(const QString & filename);

    void setLayoutType(LayoutType layoutType);
    LayoutType layoutType() const;

    void setFollowTail(bool enabled);
    bool followTail() const;

    const QString & lastSearchString() const;
    bool searchWasRegex() const;
    bool searchWasCaseSensitive() const;

    // Functions used by the LayoutStrategy class and subclasses
    FileBlockReader * blockReader() { return m_blockReader.data(); }
    std::vector<qint64> & lineAddresses() { return m_lineAddresses; }
    YTextDocument * document() { return m_document.data(); }
    void setDocumentText(const QString & data);
    void updateScrollBars(int lines, int visibleLines);
    int numLinesOnScreen() const;
    DocumentSearch * documentSearch() { return m_documentSearch.data(); }
    YFileCursor * fileCursor() { return m_fileCursor.data(); }
    const QString & filename() const { return m_filename; }
    bool searchDocument(bool isForward, bool wrapAround = true);
    void scrollToIfNecessary(const QTextCursor & cursor);

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
    void performLayout();

    void searchFile(bool isForward);

    // TODO: move to YTextCursor
    QTextCursor qTextCursor(const YFileCursor & fileCursor);
private:
    QString m_filename;
    QScopedPointer<YTextDocument> m_document;
    QScopedPointer<YFileSystemWatcherThread> m_watcher;

    // TODO: YTextDocument should own m_lineAddresses
    std::vector<qint64> m_lineAddresses;

    LayoutType m_layoutType;
    QScopedPointer<FullLayout> m_fullLayoutStrategy;
    QScopedPointer<PartialLayout> m_partialLayoutStrategy;
    LayoutStrategy * m_layoutStrategy;

    bool m_followTail;

    QScopedPointer<DocumentSearch> m_documentSearch;

    // TODO: YTextDocument should own m_fileCursor
    QScopedPointer<YFileCursor> m_fileCursor;
    QScopedPointer<FileBlockReader> m_blockReader;
};

#endif
