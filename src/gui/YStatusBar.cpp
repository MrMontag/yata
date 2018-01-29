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
            QString(msg).toHtmlEscaped() % "</p>";
    m_leftLabel->setText(format);
}

void YStatusBar::clearErrorMessage()
{
    m_leftLabel->clear();
}
