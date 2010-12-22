/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "YTabWidget.h"

#include <QContextMenuEvent>
#include <QMenu>
#include <QTabBar>
#include <QToolButton>
#include <QtDebug>

// TODO: movable tabs
// TODO: icons

YTabWidget::YTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_buttonChooseTab(new QToolButton(this))
    , m_menuTab(new QMenu(this))
    , m_tabIndexForContextMenu(-1)
{
    setDocumentMode(true);
    setTabsClosable(true);

    m_buttonChooseTab->setArrowType(Qt::DownArrow);
    m_buttonChooseTab->setPopupMode(QToolButton::InstantPopup);
    m_buttonChooseTab->setMenu(new QMenu(m_buttonChooseTab));
    setCornerWidget(m_buttonChooseTab);

    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(on_tabCloseRequested(int)));

    m_actionCloseTab = m_menuTab->addAction(
        tr("&Close tab"),
        this,
        SLOT(closeCurrentTab()),
        QKeySequence(tr("Ctrl+W")));
    m_actionCloseOtherTabs = m_menuTab->addAction(
        tr("Close &other tabs"),
        this,
        SLOT(closeAllButCurrentTab()),
        QKeySequence(tr("Ctrl+Shift+W")));
    m_actionCloseAllTabs = m_menuTab->addAction(
        tr("Close &all tabs"),
        this,
        SLOT(closeAllTabs()));

    updateContextMenu();
}

void YTabWidget::on_tabCloseRequested(int index)
{
    QWidget * to_delete = widget(index);
    removeTab(index);
    delete to_delete;
}

QMenu * YTabWidget::contextMenu()
{
    return m_menuTab;
}

void YTabWidget::openTab(QWidget * child, const QString & fullName, const QString & shortName)
{
    int index = addTab(child, shortName);
    child->setFocus();
    setTabToolTip(index, fullName);

    QAction * action = m_buttonChooseTab->menu()->addAction(fullName);
    connect(action, SIGNAL(triggered()), SLOT(on_tabChooseMenuTriggered()));

    setCurrentIndex(index);
    updateContextMenu();
}

void YTabWidget::closeCurrentTab()
{
    setCurrentTabIfNeeded();
    on_tabCloseRequested(currentIndex());
}

void YTabWidget::closeAllButCurrentTab()
{
    setCurrentTabIfNeeded();
    int current = currentIndex();
    while(current > 0) {
        on_tabCloseRequested(0);
        current--;
    }

    while(count() > 1) {
        on_tabCloseRequested(1);
    }
}

void YTabWidget::closeAllTabs()
{
    while(count() > 0) {
        on_tabCloseRequested(0);
    }
}

void YTabWidget::on_tabChooseMenuTriggered()
{
    QAction * action = dynamic_cast<QAction*>(sender());
    if(!action) { return; }

    int index = m_buttonChooseTab->menu()->actions().indexOf(action);
    setCurrentIndex(index);
}

void YTabWidget::contextMenuEvent(QContextMenuEvent * e)
{
    int tabPos = tabBar()->tabAt(e->pos());
    if(tabPos >= 0) {
        m_tabIndexForContextMenu = tabPos;
        m_menuTab->exec(e->globalPos());
    }
}

void YTabWidget::tabRemoved(int index)
{
    QMenu * chooseTabMenu = m_buttonChooseTab->menu();
    chooseTabMenu->removeAction(chooseTabMenu->actions().at(index));
    updateContextMenu();
}

void YTabWidget::setCurrentTabIfNeeded()
{
    if(m_tabIndexForContextMenu >= 0) {
        setCurrentIndex(m_tabIndexForContextMenu);
    }
    m_tabIndexForContextMenu = -1;
}

void YTabWidget::updateContextMenu()
{
    m_actionCloseTab->setEnabled(count() > 0);
    m_actionCloseOtherTabs->setEnabled(count() > 1);
    m_actionCloseAllTabs->setEnabled(count() > 0);
}
