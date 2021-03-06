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
#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>

class ColorButton : public QPushButton {
    Q_OBJECT

public:
    ColorButton(QWidget *parent = 0);
    ~ColorButton();

    const QColor & currentColor() const;
    void setCurrentColor(const QColor & color);
signals:
    void currentColorChanged(const QColor & color);

protected:
    virtual void paintEvent(QPaintEvent *e);
private slots:
    void onClick();
private:
    QColor m_currentColor;
};

#endif // COLORBUTTON_H
