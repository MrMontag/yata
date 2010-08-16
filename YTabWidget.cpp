#include "YTabWidget.h"

#include <QMenu>
#include <QTabBar>
#include <QToolButton>
#include <QtDebug>

YTabWidget::YTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_chooseTabButton(new QToolButton(this))
{
    setDocumentMode(true);
    setTabsClosable(true);

    m_chooseTabButton->setArrowType(Qt::DownArrow);
    m_chooseTabButton->setPopupMode(QToolButton::InstantPopup);
    m_chooseTabButton->setMenu(new QMenu(m_chooseTabButton));
    setCornerWidget(m_chooseTabButton);

    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(on_tabCloseRequested(int)));
}

void YTabWidget::on_tabCloseRequested(int index)
{
    removeTab(index);
}

void YTabWidget::on_tabChooseMenuTriggered()
{
    QAction * action = dynamic_cast<QAction*>(sender());
    if(!action) { return; }

    int index = m_chooseTabButton->menu()->actions().indexOf(action);
    setCurrentIndex(index);
}

void YTabWidget::tabInserted(int index)
{
    QAction * action = m_chooseTabButton->menu()->addAction(tabText(index));
    connect(action, SIGNAL(triggered()), SLOT(on_tabChooseMenuTriggered()));
}

void YTabWidget::tabRemoved(int index)
{
    QMenu * chooseTabMenu = m_chooseTabButton->menu();
    chooseTabMenu->removeAction(chooseTabMenu->actions().at(index));
}

