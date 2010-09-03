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

    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(on_tabWidget_currentChanged(int)));

    addDebugMenu();
}

// TODO: drag and drop files! :-)
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

void MainWindow::on_tabWidget_currentChanged(int index)
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
            case TailView::FullLayout: toBeChecked = m_fullLayoutAction; break;
            case TailView::PartialLayout: toBeChecked = m_partialLayoutAction; break;
            case TailView::AutomaticLayout: toBeChecked = m_automaticLayoutAction; break;
        }
        toBeChecked->setChecked(true);
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
        layoutType = TailView::FullLayout;
    } else if(senderObject == m_partialLayoutAction) {
        layoutType = TailView::PartialLayout;
    } else if(senderObject == m_automaticLayoutAction) {
        layoutType = TailView::AutomaticLayout;
    }

    if(TailView * tailView = getCurrentView()) {
        tailView->setLayoutType(layoutType);
    }
}
