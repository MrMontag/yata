/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YTABWIDGET_H
#define YTABWIDGET_H

#include <QTabWidget>
#include "app/YObjectPointer.h"

class YTabMenuButton;
class QActionGroup;
class QMenu;

class YTabWidget : public QTabWidget {
Q_OBJECT
public:
    explicit YTabWidget(QWidget *parent = 0);
    ~YTabWidget();
    void openTab(QWidget * child, const QString & fullName, const QString & shortName);
    QMenu * contextMenu();

private slots:
    void on_tabChooseMenuTriggered();
    void on_tabCloseRequested(int index);
    void closeCurrentTab();
    void closeAllButCurrentTab();
    void closeAllTabs();
    void copyFullPathToClipboard();
    void onTabMoved(int from, int to);
    void on_currentChanged(int index);

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
