#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_MainWindow.h"
#include "TailView.h"

class TailView;
class QFileSystemWatcher;

class MainWindow: public QMainWindow {
	Q_OBJECT
public:
	MainWindow();
signals:
    void fileOpened(const QString &);
private slots:
	void on_action_Open_triggered();
	void on_action_Exit_triggered();
private:
	QFileSystemWatcher * m_watcher;
	TailView * m_tailView;
	Ui::MainWindow ui;
};
#endif
