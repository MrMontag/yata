#include "DebugWindow.h"
#include "ui_DebugWindow.h"
#include "view/TailView.h"
#include "document/YTextDocument.h"

#include <QTextBlock>

DebugWindow::DebugWindow(YTabWidget* tabWidget, QWidget *parent):
    QWidget(parent),
    ui(new Ui::DebugWindow),
    m_tabWidget(tabWidget)
{
    ui->setupUi(this);
    connect(m_tabWidget.data(), SIGNAL(currentChanged(int)), SLOT(currentDocumentChanged(int)));
    currentDocumentChanged(m_tabWidget.data()->currentIndex());
}

DebugWindow::~DebugWindow()
{
}

void DebugWindow::currentDocumentChanged(int)
{
    disconnect(this, SLOT(documentContentsChanged()));
    if(TailView * view = dynamic_cast<TailView*>(m_tabWidget.data()->currentWidget())) {
        connect(view->document(), SIGNAL(contentsChanged()), SLOT(documentContentsChanged()));
        documentContentsChanged();
    }
}

void DebugWindow::documentContentsChanged()
{
    if(TailView * view = dynamic_cast<TailView*>(m_tabWidget.data()->currentWidget())) {
        const YTextDocument * doc = view->document();
        QString text;
        for(QTextBlock block = doc->begin(); block != doc->end(); block = block.next()) {
            text += block.text() + "\n";
        }
        ui->currentViewTextEdit->setText(text);
    } else {
        ui->currentViewTextEdit->clear();
    }
}
