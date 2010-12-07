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

class YTabWidget : public QTabWidget {
Q_OBJECT
public:
    explicit YTabWidget(QWidget *parent = 0);

    void openTab(QWidget * child, const QString & fullName, const QString & shortName);

private slots:
    void on_tabCloseRequested(int index);
    void on_tabChooseMenuTriggered();
private:
    void tabRemoved(int index);
private:
    QToolButton * m_chooseTabButton;
};

#endif // YTABWIDGET_H
