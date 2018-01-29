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
#ifndef TEXTCOLORWIDGET_H
#define TEXTCOLORWIDGET_H

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
    class TextColorWidget;
}

class TextColor;

class TextColorWidget : public QWidget {
    Q_OBJECT

public:
    explicit TextColorWidget(QWidget *parent = 0);
    ~TextColorWidget();

    QString text() const;
    void setText(const QString & text) const;

    const TextColor & textColor() const;
    void setTextColor(const TextColor & tc);

private slots:
    void on_defaultCheckBox_clicked(bool checked);
    void currentColorChanged(const QColor & color);

private:
    void updateColorButtons();

    Ui::TextColorWidget *ui;
    QScopedPointer<TextColor> m_textColor;
};

#endif // TEXTCOLORWIDGET_H
