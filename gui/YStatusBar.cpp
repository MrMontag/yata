/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "YStatusBar.h"
#include <QLabel>
#include <QTextDocument>
#include <QStringBuilder>

YStatusBar::YStatusBar(QWidget *parent):
    QStatusBar(parent),
    m_leftLabel(new QLabel(this))
{
    m_leftLabel->setMinimumSize(QSize(1,1));
    addWidget(m_leftLabel.data());
}

YStatusBar::~YStatusBar()
{
}

void YStatusBar::errorMessage(const QString & msg)
{
    QString format = QString("<p style=\"background-color:yellow\">") %
        Qt::escape(msg) % "</p>";
    m_leftLabel->setText(format);
}

void YStatusBar::clearErrorMessage()
{
    m_leftLabel->clear();
}
