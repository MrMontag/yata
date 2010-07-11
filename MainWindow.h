#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "TailView.h"

class TailView;

class MainWindow: public QMainWindow {
	Q_OBJECT
public:
	MainWindow();

	void setFile(const QString & filename);
signals:
    void fileOpened(const QString &);
private slots:
    void on_actionFind_previous_triggered();
    void on_actionFind_next_triggered();
    void on_action_Find_triggered();
 void on_action_Open_triggered();
	void on_action_Exit_triggered();
        void on_action_FullLayout_triggered(bool isChecked);
private:
    TailView * m_tailView;
	Ui::MainWindow ui;
};
#endif
