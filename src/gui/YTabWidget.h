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
#ifndef YTABWIDGET_H
#define YTABWIDGET_H

#include "../YObjectPointer.h"

#include <QTabWidget>

class YTabMenuButton;
class QActionGroup;
class QMenu;
class TailView;

class YTabWidget : public QTabWidget {
    Q_OBJECT
public:
    explicit YTabWidget(QWidget *parent = 0);
    ~YTabWidget();
    void openTab(TailView * child);
    QMenu * contextMenu();
    void setActive(bool active);

signals:
    void currentTabChanged(int oldIndex, int newIndex);
    void currentFileDisplayChanged();

private slots:
    void on_tabChooseMenuTriggered();
    void on_tabCloseRequested(int index);
    void closeCurrentTab();
    void closeAllButCurrentTab();
    void closeAllTabs();
    void copyFullPathToClipboard();
    void onTabMoved(int from, int to);
    void on_currentTabChanged(int index);
    void onFileChanged();

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void tabRemoved(int index);
private:
    void setCurrentTabIfNeeded();
    void updateContextMenu();
    void updateCurrentForMenuChooseTab();
    void setViewActive(int index, bool active);

private:
    YObjectPointer<YTabMenuButton> m_buttonChooseTab;
    YObjectPointer<QMenu> m_menuChooseTab;
    YObjectPointer<QActionGroup> m_actionGroupChooseTab;
    YObjectPointer<QMenu> m_menuTab;
    YObjectPointer<QAction> m_actionCloseTab;
    YObjectPointer<QAction> m_actionCloseOtherTabs;
    YObjectPointer<QAction> m_actionCloseAllTabs;
    YObjectPointer<QAction> m_actionCopyFullPathToClipboard;
    int m_tabIndexForContextMenu;
    int m_currentIndex;
};

#endif // YTABWIDGET_H
