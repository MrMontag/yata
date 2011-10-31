/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "PreferencesDialog.h"
#include "TailView.h"
#include "SearchWidget.h"
#include "SessionLoader.h"
#include "YApplication.h"
#include "YStatusBar.h"
#include "YTabWidget.h"
#include "session/FileSession.h"
#include "dbg/DebugWindow.h"

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QMimeData>
#include <QTextStream>
#include <QUrl>

// TODO: figure out disabling/enabling menu items

MainWindow::MainWindow(): ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    m_tabWidget.reset(new YTabWidget(this));
    setCentralWidget(m_tabWidget.data());

    m_statusBar.reset(new YStatusBar(this));
    setStatusBar(m_statusBar.data());

    setWindowTitle(YApplication::displayAppName());
    QString aboutYataText = ui->action_About_Yata->text();
    aboutYataText.replace("$APPNAME$", YApplication::displayAppName());
    ui->action_About_Yata->setText(aboutYataText);

    connect(m_tabWidget.data(), SIGNAL(currentChanged(int,int)), SLOT(onCurrentTabChanged(int,int)));

    ui->menu_File->insertActions(ui->action_Exit, m_tabWidget->contextMenu()->actions());
    ui->menu_File->insertSeparator(ui->action_Exit);

    setupDebugMenu();

    setAcceptDrops(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFile(const QString & filename)
{
    QFileInfo info(filename);
    QString absolute(info.absoluteFilePath());
    QString displayFilename = QDir::toNativeSeparators(absolute);
    QString displayBase = info.fileName();
    TailView * tailView = new TailView(this);
    tailView->setFile(absolute);
    m_tabWidget->openTab(tailView, displayFilename, displayBase);
}

void MainWindow::fileSessions(std::vector<FileSession> * sessions) const
{
    sessions->clear();
    for (int i = 0; i < m_tabWidget->count(); i++) {
        TailView * view = dynamic_cast<TailView*>(m_tabWidget->widget(i));
        sessions->push_back(FileSession());
        sessions->back().path = view->filename().toStdString();
    }
}

int MainWindow::currentFileIndex() const
{
    return m_tabWidget->currentIndex();
}

void MainWindow::setCurrentFileIndex(int index)
{
    m_tabWidget->setCurrentIndex(index);
}

int MainWindow::fileCount() const
{
    return m_tabWidget->count();
}

const QString & MainWindow::currentOpenDirectory() const
{
    return m_currentOpenDir;
}

void MainWindow::setCurrentOpenDirectory(const QString & dir)
{
    m_currentOpenDir = dir;
}

void MainWindow::dragEnterEvent(QDragEnterEvent * event)
{
    dragMoveEvent(event);
}

void MainWindow::dragMoveEvent(QDragMoveEvent * event)
{
    const QMimeData * mimeData = event->mimeData();
    QList<QUrl> urlList = mimeData->urls();
    for(int i = 0; i < urlList.size(); i++) {
        QString filePath = urlList.at(i).toLocalFile();
        if(!filePath.isEmpty()) {
            event->accept();
            return;
        }
    }

    event->ignore();
}

void MainWindow::dropEvent(QDropEvent * event)
{
    bool accepted = false;
    const QMimeData * mimeData = event->mimeData();
    QList<QUrl> urlList = mimeData->urls();
    for(int i = 0; i < urlList.size(); i++) {
        QString filePath = urlList.at(i).toLocalFile();
        if(!filePath.isEmpty()) {
            addFile(filePath);
            accepted = true;
        }
    }

    if(accepted) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    SessionLoader::writeSession(this);
    if(m_debugWindow) { m_debugWindow->close(); }
    e->accept();
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
    close();
}

void MainWindow::on_action_Find_triggered()
{
    SearchWidget widget(this);
    widget.exec();
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
    QTextStream(&message) <<
        YApplication::displayAppName() <<
        tr(" (Yet Another Tail Application): a universal log viewer\n\n"
           "Copyright (c) 2010-2011 James Smith");
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

void MainWindow::onCurrentTabChanged(int oldIndex, int newIndex)
{
    m_statusBar->clearErrorMessage();
    if(oldIndex != -1) {
        if(TailView * tailView = dynamic_cast<TailView*>(m_tabWidget->widget(oldIndex))) {
            disconnect(tailView, SIGNAL(fileError(QString)), m_statusBar.data(), SLOT(errorMessage(QString)));
            disconnect(tailView, SIGNAL(fileErrorCleared()), m_statusBar.data(), SLOT(clearMessage()));
        }
    }

    if(newIndex == -1) { // No tabs are open
        setWindowTitle(YApplication::displayAppName());
        return;
    }

    QString filename = m_tabWidget->tabText(newIndex);
    setWindowTitle(filename + " - " + YApplication::displayAppName());
    if(TailView * tailView = getCurrentView()) {
        QAction * toBeChecked = 0;
        switch(tailView->layoutType()) {
            case TailView::DebugFullLayout: toBeChecked = ui->action_Full_Layout; break;
            case TailView::DebugPartialLayout: toBeChecked = ui->action_Partial_Layout; break;
            case TailView::AutomaticLayout: toBeChecked = ui->action_Automatic_Layout; break;
        }
        if (toBeChecked) { toBeChecked->setChecked(true); }

        ui->actionFollow_tail->setChecked(tailView->followTail());
        connect(tailView, SIGNAL(fileError(QString)), m_statusBar.data(), SLOT(errorMessage(QString)));
        connect(tailView, SIGNAL(fileErrorCleared()), m_statusBar.data(), SLOT(clearErrorMessage()));
        if(!tailView->currentFileError().isEmpty()) {
            m_statusBar->errorMessage(tailView->currentFileError());
        }
    }
}

void MainWindow::setupDebugMenu()
{
    QActionGroup * layoutGroup(new QActionGroup(this));
    layoutGroup->addAction(ui->action_Full_Layout);
    layoutGroup->addAction(ui->action_Partial_Layout);
    layoutGroup->addAction(ui->action_Automatic_Layout);
}

void MainWindow::layoutAction(int layoutType)
{
    if(TailView * tailView = getCurrentView()) {
        tailView->setLayoutType(static_cast<TailView::LayoutType>(layoutType));
    }
}

void MainWindow::on_actionFollow_tail_triggered(bool checked)
{
    if(TailView * tailView = getCurrentView()) {
        tailView->setFollowTail(checked);
    }
}

void MainWindow::on_actionPreferences_triggered()
{
    PreferencesDialog dialog;
    dialog.exec();
}

void MainWindow::on_actionDebug_Window_triggered()
{
    m_debugWindow.reset(new DebugWindow(m_tabWidget.data()));
    connect(m_debugWindow.data(), SIGNAL(destroyed()), SLOT(debugWindowClosed()));
    m_debugWindow->show();
}

void MainWindow::debugWindowClosed()
{
    m_debugWindow.reset();
}

void MainWindow::on_action_Full_Layout_triggered()
{
    layoutAction(TailView::DebugFullLayout);
}

void MainWindow::on_action_Partial_Layout_triggered()
{
    layoutAction(TailView::DebugPartialLayout);
}

void MainWindow::on_action_Automatic_Layout_triggered()
{
    layoutAction(TailView::AutomaticLayout);
}
