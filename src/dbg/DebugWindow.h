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
#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include "../gui/YTabWidget.h"

#include <QWidget>
#include <QScopedPointer>
#include <QPointer>

namespace Ui {
    class DebugWindow;
}

class DebugWindow : public QWidget
{
    Q_OBJECT

public:
    explicit DebugWindow(YTabWidget* tabWidget, QWidget *parent = 0);
    ~DebugWindow();

private slots:
    void currentDocumentChanged(int);
    void documentContentsChanged();

private:
    QScopedPointer<Ui::DebugWindow> ui;
    QPointer<YTabWidget> m_tabWidget;
};

#endif // DEBUGWINDOW_H
