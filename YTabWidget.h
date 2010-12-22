/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef YTABWIDGET_H
#define YTABWIDGET_H

#include <QTabWidget>

class QToolButton;
class QMenu;

class YTabWidget : public QTabWidget {
Q_OBJECT
public:
    explicit YTabWidget(QWidget *parent = 0);
    void openTab(QWidget * child, const QString & fullName, const QString & shortName);
    QMenu * contextMenu();

private slots:
    void on_tabChooseMenuTriggered();
    void on_tabCloseRequested(int index);
    void closeCurrentTab();
    void closeAllButCurrentTab();
    void closeAllTabs();

protected:
    void contextMenuEvent(QContextMenuEvent *);
    void tabRemoved(int index);
private:
    void setCurrentTabIfNeeded();
    void updateContextMenu();

private:
    QToolButton * m_buttonChooseTab;
    QMenu * m_menuTab;
    QAction * m_actionCloseTab;
    QAction * m_actionCloseOtherTabs;
    QAction * m_actionCloseAllTabs;
    int m_tabIndexForContextMenu;
};

#endif // YTABWIDGET_H
