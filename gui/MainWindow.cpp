/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "preferences/Preferences.h"
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
#include <QStyle>
#include <QTextStream>
#include <QUrl>

MainWindow::MainWindow(): ui(new Ui::MainWindow())
{
    ui->setupUi(this);
    m_tabWidget.reset(new YTabWidget(this));
    setCentralWidget(m_tabWidget.data());

    m_statusBar.reset(new YStatusBar(this));
    setStatusBar(m_statusBar.data());

    setWindowTitle(YApplication::displayAppName());
    QString aboutYataText = ui->actionAboutYata->text();
    aboutYataText.replace("$APPNAME$", YApplication::displayAppName());
    ui->actionAboutYata->setText(aboutYataText);

    connect(m_tabWidget.data(), SIGNAL(currentChanged(int,int)), SLOT(onCurrentTabChanged(int,int)));

    ui->menuFile->insertActions(ui->actionExit, m_tabWidget->contextMenu()->actions());
    ui->menuFile->insertSeparator(ui->actionExit);
    ui->actionOpen->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));

    setupDebugMenu();
    setAcceptDrops(true);
    updateMenus();
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

void MainWindow::on_actionOpen_triggered()
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

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_actionFind_triggered()
{
    SearchWidget widget(this);
    widget.exec();
}

void MainWindow::on_actionFindNext_triggered()
{
    if(TailView * tailView = getCurrentView()) {
        tailView->searchForward();
    }
}

void MainWindow::on_actionFindPrevious_triggered()
{
    if(TailView * tailView = getCurrentView()) {
        tailView->searchBackward();
    }
}

void MainWindow::on_actionAboutQt_triggered()
{
   QApplication::aboutQt();
}

void MainWindow::on_actionAboutYata_triggered()
{
    QString title;
    QTextStream(&title) << tr("About ") << YApplication::displayAppName();

    QString message;
    QTextStream(&message) <<
        "<h3>" << YApplication::displayAppName() << "</h3>" <<
        tr("(Yet Another Tail Application)<br>") <<
        tr("A universal log viewer<br>") <<
        tr("Version ") << YApplication::appVersion() <<
        tr("<br><br>Copyright (c) 2010-2012 James Smith<br><br>"
           "http://code.google.com/p/yata");
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
    updateMenus();
    if(oldIndex != -1) {
        if(TailView * tailView = dynamic_cast<TailView*>(m_tabWidget->widget(oldIndex))) {
            // disconnect file error slots
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
        // Set debugging menu
        QAction * toBeChecked = 0;
        switch(tailView->layoutType()) {
            case TailView::DebugFullLayout: toBeChecked = ui->actionFullLayout; break;
            case TailView::DebugPartialLayout: toBeChecked = ui->actionPartialLayout; break;
            case TailView::AutomaticLayout: toBeChecked = ui->actionAutomaticLayout; break;
        }
        if (toBeChecked) { toBeChecked->setChecked(true); }

        // Follow tail setting
        ui->actionFollowTail->setChecked(tailView->followTail());

        // Connect error signals
        connect(tailView, SIGNAL(fileError(QString)), m_statusBar.data(), SLOT(errorMessage(QString)));
        connect(tailView, SIGNAL(fileErrorCleared()), m_statusBar.data(), SLOT(clearErrorMessage()));
        if(!tailView->currentFileError().isEmpty()) {
            m_statusBar->errorMessage(tailView->currentFileError());
        }
    }
}

void MainWindow::setupDebugMenu()
{
    if(Preferences::instance()->debugMenu()) {
        QActionGroup * layoutGroup(new QActionGroup(this));
        layoutGroup->addAction(ui->actionFullLayout);
        layoutGroup->addAction(ui->actionPartialLayout);
        layoutGroup->addAction(ui->actionAutomaticLayout);
    } else {
        menuBar()->removeAction(ui->menuDebug->menuAction());
    }
}

void MainWindow::layoutAction(int layoutType)
{
    if(TailView * tailView = getCurrentView()) {
        tailView->setLayoutType(static_cast<TailView::LayoutType>(layoutType));
    }
}

void MainWindow::updateMenus()
{
    rebuildEditMenu();
    bool enabled = (getCurrentView() != 0);
    ui->actionFind->setEnabled(enabled);
    ui->actionFindNext->setEnabled(enabled);
    ui->actionFindPrevious->setEnabled(enabled);
    ui->actionRefresh->setEnabled(enabled);
    ui->actionFollowTail->setEnabled(enabled);
}

void MainWindow::rebuildEditMenu()
{
    ui->menuEdit->clear();
    if(TailView * view = getCurrentView()) {
        ui->menuEdit->addActions(view->actions());
        ui->menuEdit->addSeparator();
    }

    ui->menuEdit->addAction(ui->actionFind);
    ui->menuEdit->addAction(ui->actionFindNext);
    ui->menuEdit->addAction(ui->actionFindPrevious);
    ui->menuEdit->addSeparator();
    ui->menuEdit->addAction(ui->actionPreferences);
}

void MainWindow::on_actionFollowTail_triggered(bool checked)
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

void MainWindow::on_actionDebugWindow_triggered()
{
    m_debugWindow.reset(new DebugWindow(m_tabWidget.data()));
    connect(m_debugWindow.data(), SIGNAL(destroyed()), SLOT(debugWindowClosed()));
    m_debugWindow->show();
}

void MainWindow::debugWindowClosed()
{
    m_debugWindow.reset();
}

void MainWindow::on_actionFullLayout_triggered()
{
    layoutAction(TailView::DebugFullLayout);
}

void MainWindow::on_actionPartialLayout_triggered()
{
    layoutAction(TailView::DebugPartialLayout);
}

void MainWindow::on_actionAutomaticLayout_triggered()
{
    layoutAction(TailView::AutomaticLayout);
}
