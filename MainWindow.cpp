#include "MainWindow.h"
#include "TailView.h"
#include "SearchWidget.h"
#include <QFileDialog>
#include <QFileInfo>

MainWindow::MainWindow()
{
    ui.setupUi(this);
    m_tailView = new TailView(this);
    setCentralWidget(m_tailView);
    ui.action_FullLayout->setChecked(m_tailView->isFullLayout());
}

// TODO: drag and drop files! :-)
void MainWindow::setFile(const QString & filename)
{
    setWindowTitle(QDir::toNativeSeparators(filename) + " -- Yata");
    m_tailView->setFile(filename);
}


void MainWindow::on_action_Open_triggered()
{
    // TODO: allow to open non-existant file
    QString filename = QFileDialog::getOpenFileName(this, QString(), m_currentOpenDir);
    if(!filename.isEmpty()) {
        setFile(filename);
        m_currentOpenDir = QFileInfo(filename).path();
    }
}

void MainWindow::on_action_Exit_triggered()
{
    qApp->exit();
}

void MainWindow::on_action_FullLayout_triggered(bool isChecked)
{
    m_tailView->setFullLayout(isChecked);
}

void MainWindow::on_action_Find_triggered()
{
    SearchWidget widget(m_tailView->lastSearchString(), m_tailView->searchWasRegex(), m_tailView->searchWasCaseSensitive(), this);
    connect(&widget, SIGNAL(searchAccepted(QString,bool,bool)), m_tailView, SLOT(newSearch(const QString &,bool,bool)));
    widget.exec();
}

void MainWindow::on_actionFind_next_triggered()
{
    m_tailView->searchForward();
}

void MainWindow::on_actionFind_previous_triggered()
{
    m_tailView->searchBackward();
}
