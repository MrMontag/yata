#include "MainWindow.h"
#include "TailView.h"
#include "YTabWidget.h"
#include "SearchWidget.h"
#include "YApplication.h"
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow()
{
    ui.setupUi(this);
    m_tabWidget = new YTabWidget(this);
    setCentralWidget(m_tabWidget);

    setWindowTitle(YApplication::displayAppName());
    QString aboutYataText = ui.action_About_Yata->text();
    aboutYataText.replace("$APPNAME$", YApplication::displayAppName());
    ui.action_About_Yata->setText(aboutYataText);

    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(on_tabWidget_currentChanged(int)));
}

// TODO: drag and drop files! :-)
void MainWindow::addFile(const QString & filename)
{
    QString displayFilename = QDir::toNativeSeparators(filename);
    TailView * tailView = new TailView(this);
    tailView->setFile(filename);
    tailView->setFullLayout(ui.action_FullLayout->isChecked());
    int index = m_tabWidget->addTab(tailView, displayFilename);
    m_tabWidget->setCurrentIndex(index);
}


void MainWindow::on_action_Open_triggered()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
        this,
        tr("Choose File"),
        m_currentOpenDir);
    if(!filenames.isEmpty()) {
        QString filename;
        foreach(filename, filenames) {
            addFile(filename);
            m_currentOpenDir = QFileInfo(filename).path();
        }
    }
}

void MainWindow::on_action_Exit_triggered()
{
    qApp->exit();
}

void MainWindow::on_action_FullLayout_triggered(bool isChecked)
{
    if(TailView * tailView = getCurrentView()) {
        tailView->setFullLayout(isChecked);
    }
}

void MainWindow::on_action_Find_triggered()
{
    if(TailView * tailView = getCurrentView()) {
        SearchWidget widget(tailView->lastSearchString(), tailView->searchWasRegex(), tailView->searchWasCaseSensitive(), this);
        connect(&widget, SIGNAL(searchAccepted(QString,bool,bool)), tailView, SLOT(newSearch(const QString &,bool,bool)));
        widget.exec();
    }
}

void MainWindow::on_actionFind_next_triggered()
{
    if(TailView * tailView = getCurrentView()) {
        tailView->searchForward();
    }
}

void MainWindow::on_actionFind_previous_triggered()
{
    if(TailView * tailView = getCurrentView()) {
        tailView->searchBackward();
    }
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
    if(TailView * tailView = getCurrentView()) {
        tailView->onFileChanged();
    }
}

TailView * MainWindow::getCurrentView()
{
    return dynamic_cast<TailView*>(m_tabWidget->currentWidget());
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    QString filename = m_tabWidget->tabText(index);
    setWindowTitle(filename + " - " + YApplication::displayAppName());
    TailView * tailView = getCurrentView();
    ui.action_FullLayout->setChecked(tailView->isFullLayout());
}
