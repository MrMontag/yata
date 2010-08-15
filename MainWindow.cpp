#include "MainWindow.h"
#include "TailView.h"
#include "SearchWidget.h"
#include "YApplication.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow()
{
    ui.setupUi(this);
    m_tailView = new TailView(this);
    setCentralWidget(m_tailView);
    ui.action_FullLayout->setChecked(m_tailView->isFullLayout());

    setWindowTitle(YApplication::displayAppName());
    QString aboutYataText = ui.action_About_Yata->text();
    aboutYataText.replace("$APPNAME$", YApplication::displayAppName());
    ui.action_About_Yata->setText(aboutYataText);
}

// TODO: drag and drop files! :-)
void MainWindow::setFile(const QString & filename)
{
    setWindowTitle(QDir::toNativeSeparators(filename) + " - " + YApplication::displayAppName());
    m_tailView->setFile(filename);
}


void MainWindow::on_action_Open_triggered()
{
    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("Choose File"),
        m_currentOpenDir,
        QString(),
        0,
        QFileDialog::DontConfirmOverwrite);
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

void MainWindow::on_actionAbout_Qt_triggered()
{
   QApplication::aboutQt();
}

void MainWindow::on_action_About_Yata_triggered()
{
    QString title;
    QTextStream(&title) << tr("About ") << YApplication::displayAppName();

    QString message;
    QTextStream(&message) << YApplication::displayAppName() << tr(": Copyright (c) 2010 James Smith");
    QMessageBox::about(this, title, message);
}

void MainWindow::on_actionRefresh_triggered()
{
    m_tailView->onFileChanged();
}
