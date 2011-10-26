/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "app/YObjectPointer.h"
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

    void closeEvent(QCloseEvent *);
private slots:
    void on_actionFollow_tail_triggered(bool checked);
    void on_actionRefresh_triggered();
    void on_action_About_Yata_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionFind_previous_triggered();
    void on_actionFind_next_triggered();
    void on_action_Find_triggered();
    void on_action_Open_triggered();
	void on_action_Exit_triggered();

    void on_actionPreferences_triggered();

    void on_action_Full_Layout_triggered();
    void on_action_Partial_Layout_triggered();
    void on_action_Automatic_Layout_triggered();
    void on_actionDebug_Window_triggered();

    void debugWindowClosed();
    void onCurrentTabChanged(int oldIndex, int newIndex);
private:
    TailView * getCurrentView();
    void setupDebugMenu();
    void layoutAction(int layoutType); // declared int to avoid having to #include "TailView.h"
private:
    Ui::MainWindow * ui;

    YObjectPointer<YTabWidget> m_tabWidget;
    YObjectPointer<YStatusBar> m_statusBar;
    QString m_currentOpenDir;

    YObjectPointer<DebugWindow> m_debugWindow;
};
#endif
