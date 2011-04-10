/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "MainWindow.h"
#include "TailView.h"
#include "YTabWidget.h"
#include "SearchWidget.h"
#include "SessionLoader.h"
#include "YApplication.h"
#include "session/FileSession.h"

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

MainWindow::MainWindow()
    : m_fullLayoutAction(0)
    , m_partialLayoutAction(0)
    , m_automaticLayoutAction(0)
{
    ui.setupUi(this);
    m_tabWidget = new YTabWidget(this);
    setCentralWidget(m_tabWidget);

    setWindowTitle(YApplication::displayAppName());
    QString aboutYataText = ui.action_About_Yata->text();
    aboutYataText.replace("$APPNAME$", YApplication::displayAppName());
    ui.action_About_Yata->setText(aboutYataText);

    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(onCurrentTabChanged(int)));

    ui.menu_File->insertActions(ui.action_Exit, m_tabWidget->contextMenu()->actions());
    ui.menu_File->insertSeparator(ui.action_Exit);

    addDebugMenu();

    setAcceptDrops(true);
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
    QTextStream(&message) <<
        YApplication::displayAppName() <<
        tr(" (Yet Another Tail Application): a universal log viewer\n\n"
           "Copyright (c) 2010 James Smith");
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

void MainWindow::onCurrentTabChanged(int index)
{
    if(index == -1) { // No tabs are open
        setWindowTitle(YApplication::displayAppName());
        return;
    }

    QString filename = m_tabWidget->tabText(index);
    setWindowTitle(filename + " - " + YApplication::displayAppName());
    if(TailView * tailView = getCurrentView()) {
        QAction * toBeChecked = 0;
        switch(tailView->layoutType()) {
            case TailView::DebugFullLayout: toBeChecked = m_fullLayoutAction; break;
            case TailView::DebugPartialLayout: toBeChecked = m_partialLayoutAction; break;
            case TailView::AutomaticLayout: toBeChecked = m_automaticLayoutAction; break;
        }
        toBeChecked->setChecked(true);

        ui.actionFollow_tail->setChecked(tailView->followTail());
    }
}

void MainWindow::addDebugMenu()
{
    QMenu * debugMenu = menuBar()->addMenu("&Debug");
    QActionGroup * layoutGroup(new QActionGroup(this));

    m_fullLayoutAction =
        layoutGroup->addAction(debugMenu->addAction("&Full layout", this, SLOT(layoutAction())));
    m_partialLayoutAction =
        layoutGroup->addAction(debugMenu->addAction("&Partial layout", this, SLOT(layoutAction())));
    m_automaticLayoutAction =
        layoutGroup->addAction(debugMenu->addAction("&Automatic layout", this, SLOT(layoutAction())));

    QAction * itr = 0;
    foreach(itr, layoutGroup->actions()) {
        itr->setCheckable(true);
    }
}

void MainWindow::layoutAction()
{
    QObject * senderObject = sender();
    TailView::LayoutType layoutType = TailView::AutomaticLayout;
    if(senderObject == m_fullLayoutAction) {
        layoutType = TailView::DebugFullLayout;
    } else if(senderObject == m_partialLayoutAction) {
        layoutType = TailView::DebugPartialLayout;
    } else if(senderObject == m_automaticLayoutAction) {
        layoutType = TailView::AutomaticLayout;
    }

    if(TailView * tailView = getCurrentView()) {
        tailView->setLayoutType(layoutType);
    }
}

void MainWindow::on_actionFollow_tail_triggered(bool checked)
{
   if(TailView * tailView = getCurrentView()) {
       tailView->setFollowTail(checked);
   }
}
