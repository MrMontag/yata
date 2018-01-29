/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TAIL_VIEW_H
#define TAIL_VIEW_H

#include <QAbstractScrollArea>
#include <QScopedPointer>
#include <vector>
#include "../YObjectPointer.h"

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
    void updateScrollBars(int newMax);
    int numLinesOnScreen() const;
    DocumentSearch * documentSearch() { return m_documentSearch.data(); }
    const QString & filename() const { return m_filename; }
    QString longDisplayTitle() const;
    QString displayTitle() const;
    bool searchDocument(bool isForward);
    void scrollToIfNecessary(const YFileCursor & ycursor);
    ScrollBarStrategy * scrollBarStrategy();

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
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void paintEvent(QPaintEvent * event);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent * event);
    void wheelEvent(QWheelEvent * event);

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
