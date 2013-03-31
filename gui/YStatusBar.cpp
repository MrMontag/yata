/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
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
    // Set a small minimum size so the label won't prevent the window from resizing smaller that its text.
    m_leftLabel->setMinimumSize(QSize(1,1));
    addWidget(m_leftLabel.data());
}

YStatusBar::~YStatusBar()
{
}

void YStatusBar::errorMessage(const QString & msg)
{
    QString format = QString("<p style=\"background-color:yellow\">") %
        msg.toHtmlEscaped() % "</p>";
    m_leftLabel->setText(format);
}

void YStatusBar::clearErrorMessage()
{
    m_leftLabel->clear();
}
