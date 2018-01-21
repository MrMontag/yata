/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
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
