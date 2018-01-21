/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef YSTATUSBAR_H
#define YSTATUSBAR_H

#include <QStatusBar>
#include "../YObjectPointer.h"

class QLabel;

class YStatusBar: public QStatusBar {
    Q_OBJECT

public:
    explicit YStatusBar(QWidget *parent = 0);
    ~YStatusBar();

public slots:
    void errorMessage(const QString & msg);
    void clearErrorMessage();

private:
    YObjectPointer<QLabel> m_leftLabel;
};

#endif // YSTATUSBAR_H
