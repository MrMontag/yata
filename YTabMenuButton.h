/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef WIDGET_H
#define WIDGET_H

#include <QToolButton>

class YTabMenuButton : public QToolButton {
public:
    YTabMenuButton(QWidget *parent = 0);
    ~YTabMenuButton();
protected:
    void paintEvent(QPaintEvent *);
};

#endif // WIDGET_H
