/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>
#include <QScopedPointer>
#include <vector>
#include "app/YObjectPointer.h"

class DocumentSearch;
class PartialLayout;
class QString;
class QTextBlock;
class SearchCriteria;
class YFileCursor;
class YFileSystemWatcherThread;
class YTextDocument;
class ScrollBarStrategy;
class ExactScrollBarController;
class ApproximateScrollBarController;

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

    void setActive(bool active);

    // Search functions
    const QString & lastSearchString() const;
    bool searchWasRegex() const;
    bool searchWasCaseSensitive() const;

    // Functions used by the LayoutStrategy class and subclasses
    YTextDocument * document() { return m_document.data(); }
    int numLinesOnScreen() const;
    DocumentSearch * documentSearch() { return m_documentSearch.data(); }
    const QString & filename() const { return m_filename; }
    QString longDisplayTitle() const;
    QString displayTitle() const;
    bool searchDocument(bool isForward, bool wrapAround = true);
    void scrollToIfNecessary(const YFileCursor & ycursor);

    const QString & currentFileError() const { return m_currentFileError; }

signals:
    void fileError(const QString &);
    void fileErrorCleared();
    void fileChanged();

public slots:
    void newSearch();
    void searchForward();
    void searchBackward();
    void onFileChanged();
    void onCopy(bool x11Selection = false);
    void onSelectAll();

protected:
    void mousePressEvent(QMouseEvent * event) override;
    void mouseReleaseEvent(QMouseEvent * event) override;
    void mouseMoveEvent(QMouseEvent * event) override;
    void mouseDoubleClickEvent(QMouseEvent * event) override;
    void paintEvent(QPaintEvent * event) override;
    void resizeEvent(QResizeEvent *) override;
    void keyPressEvent(QKeyEvent * event) override;
    void wheelEvent(QWheelEvent * event) override;

private slots:
    void vScrollBarAction(int action);
    void onPreferencesChanged();

private:
    void searchFile(bool isForward);
    QPoint docGraphicalPosition(const QPoint & viewPoint);

private:
    QString m_filename;
    QScopedPointer<YTextDocument> m_document;
    YObjectPointer<YFileSystemWatcherThread> m_watcher;

    bool m_isActive;
    bool m_leftMouseIsDown;
    bool m_hasUnviewedChanges;
    QString m_currentFileError;

    LayoutType m_layoutType;
    QScopedPointer<PartialLayout> m_layoutStrategy;

    QScopedPointer<ExactScrollBarController> m_exactScrollBarController;
    QScopedPointer<ApproximateScrollBarController> m_approximateScrollBarController;
    ScrollBarStrategy * m_scrollBarStrategy;

    bool m_followTail;

    QScopedPointer<DocumentSearch> m_documentSearch;
};

#endif
