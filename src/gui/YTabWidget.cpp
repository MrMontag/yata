/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gui/YTabWidget.h"
#include "gui/YTabMenuButton.h"
#include "view/TailView.h"

#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QContextMenuEvent>
#include <QDir>
#include <QMenu>
#include <QTabBar>

// TODO: icons

YTabWidget::YTabWidget(QWidget *parent)
    : QTabWidget(parent)
    , m_buttonChooseTab(new YTabMenuButton(this))
    , m_menuChooseTab(new QMenu(this))
    , m_actionGroupChooseTab(new QActionGroup(this))
    , m_menuTab(new QMenu(this))
    , m_tabIndexForContextMenu(-1)
    , m_currentIndex(-1)
{
    setDocumentMode(true);
    setTabsClosable(true);

    m_buttonChooseTab->setMenu(m_menuChooseTab.data());
    setCornerWidget(m_buttonChooseTab.data());
    setMovable(true);

    connect(this, SIGNAL(tabCloseRequested(int)), SLOT(on_tabCloseRequested(int)));
    connect(tabBar(), SIGNAL(tabMoved(int,int)), SLOT(onTabMoved(int,int)));
    connect(this, SIGNAL(currentChanged(int)), SLOT(on_currentTabChanged(int)));

    m_actionCloseTab.reset(m_menuTab->addAction(
        tr("&Close tab"),
        this,
        SLOT(closeCurrentTab()),
        QKeySequence(tr("Ctrl+W"))));
    m_actionCloseOtherTabs.reset(m_menuTab->addAction(
        tr("Close &other tabs"),
        this,
        SLOT(closeAllButCurrentTab()),
        QKeySequence(tr("Ctrl+Shift+W"))));
    m_actionCloseAllTabs.reset(m_menuTab->addAction(
        tr("Close &all tabs"),
        this,
        SLOT(closeAllTabs())));
    m_menuTab->addSeparator();
    m_actionCopyFullPathToClipboard.reset(m_menuTab->addAction(
        tr("Copy full &path to clipboard"),
        this,
        SLOT(copyFullPathToClipboard())));

    updateContextMenu();
}

YTabWidget::~YTabWidget()
{
}

void YTabWidget::on_tabCloseRequested(int index)
{
    QWidget * to_delete = widget(index);
    removeTab(index);
    delete to_delete;
}

QMenu * YTabWidget::contextMenu()
{
    return m_menuTab.data();
}

void YTabWidget::setActive(bool active)
{
    if (TailView * view = dynamic_cast<TailView*>(currentWidget())) {
        view->setActive(active);
        setTabText(currentIndex(), view->displayTitle());
        emit currentFileDisplayChanged();
    }
}

void YTabWidget::openTab(TailView * child)
{
    QString fullName = child->longDisplayTitle();
    QString shortName = child->displayTitle();
    int index = addTab(child, shortName);
    child->setFocus();
    setTabToolTip(index, fullName);
    connect(child, SIGNAL(fileChanged()), SLOT(onFileChanged()));

    QAction * action = new QAction(fullName, this);
    action->setCheckable(true);
    m_menuChooseTab->addAction(action);
    m_actionGroupChooseTab->addAction(action);
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

void YTabWidget::copyFullPathToClipboard()
{
    if(TailView * view = dynamic_cast<TailView*>(widget(m_tabIndexForContextMenu))) {
        QApplication::clipboard()->setText(QDir::toNativeSeparators(view->filename()));
    }
}

void YTabWidget::onTabMoved(int from, int to)
{
    QList<QAction*> actions = m_menuChooseTab->actions();
    actions.move(from, to);
    QMenu * newMenu = new QMenu;
    newMenu->addActions(actions);
    m_buttonChooseTab->setMenu(newMenu);
    m_menuChooseTab.reset(newMenu);
    m_currentIndex = currentIndex();
}

void YTabWidget::on_currentTabChanged(int index)
{
    if(m_currentIndex > -1) {
        setViewActive(m_currentIndex, false);
    }
    setViewActive(index, true);
    int oldIndex = m_currentIndex;
    m_currentIndex = index;

    updateCurrentForMenuChooseTab();
    emit currentTabChanged(oldIndex, m_currentIndex);
}

void YTabWidget::onFileChanged()
{
    TailView * view = dynamic_cast<TailView*>(sender());
    int index = indexOf(view);
    if (index < 0) { return; }

    if (index == currentIndex() && isActiveWindow()) { return; }

    setTabText(index, view->displayTitle());
    if (index == currentIndex()) {
        emit currentFileDisplayChanged();
    }
}

void YTabWidget::on_tabChooseMenuTriggered()
{
    QAction * action = dynamic_cast<QAction*>(sender());
    if(!action) { return; }

    int index = m_menuChooseTab->actions().indexOf(action);
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
    QAction * action = m_menuChooseTab->actions().at(index);
    m_menuChooseTab->removeAction(action);
    m_actionGroupChooseTab->removeAction(action);
    delete action;

    updateContextMenu();
    updateCurrentForMenuChooseTab();
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
    m_actionCopyFullPathToClipboard->setEnabled(count() > 0);
}

void YTabWidget::updateCurrentForMenuChooseTab()
{
    QList<QAction*> actions = m_menuChooseTab->actions();
    if(m_currentIndex >= 0 && m_currentIndex < actions.size()) {
        actions[m_currentIndex]->setChecked(true);
    }
}

void YTabWidget::setViewActive(int index, bool active)
{
    if(TailView * view = dynamic_cast<TailView*>(widget(index))) {
        view->setActive(active);
        setTabText(index, view->displayTitle());
    }
}
