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
#include "ColorButton.h"
#include <QBrush>
#include <QColorDialog>
#include <QPainter>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOptionButton>

ColorButton::ColorButton(QWidget *parent):
    QPushButton(parent)
{
    connect(this, SIGNAL(clicked()), SLOT(onClick()));
}

ColorButton::~ColorButton()
{
}

const QColor & ColorButton::currentColor() const
{
    return m_currentColor;
}

void ColorButton::setCurrentColor(const QColor & color)
{
    m_currentColor = color;
    update();
}

void ColorButton::paintEvent(QPaintEvent * e)
{
    QPushButton::paintEvent(e);
    QPainter painter(this);
    QStyleOptionButton option;
    initStyleOption(&option);
    QRect recti = style()->subElementRect(QStyle::SE_PushButtonContents, &option);
    painter.fillRect(recti, m_currentColor);

    QColor border(isEnabled() ? Qt::black : Qt::gray);
    painter.setPen(QPen(QBrush(border), 1));
    recti.adjust(0,0,-1,-1);
    painter.drawRect(recti);
}

void ColorButton::onClick()
{
    QColorDialog dialog(m_currentColor, this);
    int result = dialog.exec();
    if(result == QColorDialog::Accepted) {
        m_currentColor = dialog.currentColor();
        update();
        emit currentColorChanged(m_currentColor);
    }
}
