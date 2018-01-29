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
#include "DebugWindow.h"
#include "ui_DebugWindow.h"
#include "../view/TailView.h"
#include "../document/YTextDocument.h"

#include <QTextBlock>

DebugWindow::DebugWindow(YTabWidget* tabWidget, QWidget *parent):
    QWidget(parent),
    ui(new Ui::DebugWindow),
    m_tabWidget(tabWidget)
{
    ui->setupUi(this);
    connect(m_tabWidget, SIGNAL(currentChanged(int)), SLOT(currentDocumentChanged(int)));
    currentDocumentChanged(m_tabWidget->currentIndex());
}

DebugWindow::~DebugWindow()
{
}

void DebugWindow::currentDocumentChanged(int)
{
    disconnect(this, SLOT(documentContentsChanged()));
    if(TailView * view = dynamic_cast<TailView*>(m_tabWidget->currentWidget())) {
        connect(view->document(), SIGNAL(contentsChanged()), SLOT(documentContentsChanged()));
        documentContentsChanged();
    }
}

void DebugWindow::documentContentsChanged()
{
    if(TailView * view = dynamic_cast<TailView*>(m_tabWidget->currentWidget())) {
        const YTextDocument * doc = view->document();
        QString text;
        for(QTextBlock block = doc->begin(); block != doc->end(); block = block.next()) {
            text += block.text() + "\n";
        }
        ui->currentViewTextEdit->setText(text);
    } else {
        ui->currentViewTextEdit->clear();
    }
}
