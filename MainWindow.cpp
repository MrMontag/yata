#include "MainWindow.h"
#include "TailView.h"
#include <QVBoxLayout>
#include <QFileSystemWatcher>
#include <QFileDialog>

MainWindow::MainWindow()
{
	ui.setupUi(this);
	m_watcher = new QFileSystemWatcher;
	m_tailView = new TailView(this);
	setCentralWidget(m_tailView);
	connect(m_watcher, SIGNAL(fileChanged(const QString &)), m_tailView, SLOT(onFileChanged(const QString &)));
	connect(this, SIGNAL(fileOpened(const QString &)), m_tailView, SLOT(onFileChanged(const QString &)));
}

void MainWindow::on_action_Open_triggered()
{
	QString filename = QFileDialog::getOpenFileName(this);
	qDebug("\"%s\"", filename.toStdString().c_str());
	if(!filename.isEmpty()) {
		m_watcher->removePaths(m_watcher->files());
		m_watcher->addPath(filename);
		emit fileOpened(filename);
	}	
}

void MainWindow::on_action_Exit_triggered()
{
	qApp->exit();
}

