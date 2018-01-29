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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../YObjectPointer.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class DebugWindow;
class FileSession;
class TailView;
class YTabWidget;
class YStatusBar;

class MainWindow: public QMainWindow {
    Q_OBJECT
public:
    MainWindow();
    ~MainWindow();

    void addFile(const QString & filename);

    void fileSessions(std::vector<FileSession> * sessions) const;
    int currentFileIndex() const;
    void setCurrentFileIndex(int index);
    int fileCount() const;

    const QString & currentOpenDirectory() const;
    void setCurrentOpenDirectory(const QString & dir);

signals:
    void fileOpened(const QString &);
protected:
    void dragEnterEvent(QDragEnterEvent *);
    void dragMoveEvent(QDragMoveEvent *);
    void dropEvent(QDropEvent *);
    void changeEvent(QEvent *);

    void closeEvent(QCloseEvent *);
private slots:
    void on_actionFollowTail_triggered(bool checked);
    void on_actionRefresh_triggered();
    void on_actionAboutYata_triggered();
    void on_actionAboutQt_triggered();
    void on_actionFindPrevious_triggered();
    void on_actionFindNext_triggered();
    void on_actionFind_triggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();

    void on_actionPreferences_triggered();

    void on_actionFullLayout_triggered();
    void on_actionPartialLayout_triggered();
    void on_actionAutomaticLayout_triggered();
    void on_actionDebugWindow_triggered();

    void debugWindowClosed();
    void onCurrentTabChanged(int oldIndex, int);
    void updateWindowTitle();

private:
    TailView * getCurrentView();
    void setupDebugMenu();
    void layoutAction(int layoutType); // declared int to avoid having to #include "TailView.h"
    void updateMenus();
    void rebuildEditMenu();
private:
    Ui::MainWindow * ui;

    YObjectPointer<YTabWidget> m_tabWidget;
    YObjectPointer<YStatusBar> m_statusBar;
    QString m_currentOpenDir;

    YObjectPointer<DebugWindow> m_debugWindow;
};
#endif
