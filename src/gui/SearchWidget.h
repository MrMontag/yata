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
#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include <QDialog>
#include <QScopedPointer>

namespace Ui {
    class SearchWidget;
}

class SearchWidget: public QDialog {
    Q_OBJECT
public:
    SearchWidget(QWidget *parent = 0);
    ~SearchWidget();
signals:
    void searchAccepted(const QString & searchExpression, bool isRegex, bool caseSensitive);

public slots:
    void accept();

protected:
    void changeEvent(QEvent *e);

private:
    QScopedPointer<Ui::SearchWidget> ui;
};

#endif // SEARCHWIDGET_H
