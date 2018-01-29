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
#include "YTabMenuButton.h"
#include <QToolButton>
#include <QMenu>
#include <QPainter>
#include <QStyleOptionToolButton>

YTabMenuButton::YTabMenuButton(QWidget *parent)
    : QToolButton(parent)
{
    setArrowType(Qt::DownArrow);
    setPopupMode(QToolButton::InstantPopup);
}

YTabMenuButton::~YTabMenuButton()
{
}

void YTabMenuButton::paintEvent(QPaintEvent *)
{
    QStyleOptionToolButton option;
    initStyleOption(&option);
    option.features &= ~QStyleOptionToolButton::HasMenu;

    QPainter painter(this);
    style()->drawComplexControl(QStyle::CC_ToolButton, &option, &painter, this);
}
