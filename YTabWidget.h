/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YTABWIDGET_H
#define YTABWIDGET_H

#include <QTabWidget>
#include <QScopedPointer>

class YTabMenuButton;
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
    void onTabMoved(int from, int to);
    void on_currentChanged(int index);

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void tabRemoved(int index);
private:
    void setCurrentTabIfNeeded();
    void updateContextMenu();
    void setViewActive(int index, bool active);

private:
    YTabMenuButton * m_buttonChooseTab;
    QScopedPointer<QMenu> m_menuChooseTab;
    QMenu * m_menuTab;
    QAction * m_actionCloseTab;
    QAction * m_actionCloseOtherTabs;
    QAction * m_actionCloseAllTabs;
    int m_tabIndexForContextMenu;
    int m_currentIndex;
};

#endif // YTABWIDGET_H
