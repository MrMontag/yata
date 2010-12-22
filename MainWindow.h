/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"

class TailView;
class YTabWidget;

class MainWindow: public QMainWindow {
	Q_OBJECT
public:
	MainWindow();

	void addFile(const QString & filename);
signals:
    void fileOpened(const QString &);
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
    void onCurrentTabChanged(int index);

    void layoutAction();
private:
    TailView * getCurrentView();
    void addDebugMenu();
private:
    YTabWidget * m_tabWidget;
    QString m_currentOpenDir;
	Ui::MainWindow ui;

    QAction * m_fullLayoutAction;
    QAction * m_partialLayoutAction;
    QAction * m_automaticLayoutAction;
};
#endif
