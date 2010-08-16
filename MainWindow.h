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
    void on_actionRefresh_triggered();
    void on_action_About_Yata_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionFind_previous_triggered();
    void on_actionFind_next_triggered();
    void on_action_Find_triggered();
    void on_action_Open_triggered();
	void on_action_Exit_triggered();
    void on_action_FullLayout_triggered(bool isChecked);
    void on_tabWidget_currentChanged(int index);
private:
    TailView * getCurrentView();
private:
    YTabWidget * m_tabWidget;
    QString m_currentOpenDir;
	Ui::MainWindow ui;
};
#endif
