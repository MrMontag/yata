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
#ifndef EXACTSCROLLBARCONTROLLER_H
#define EXACTSCROLLBARCONTROLLER_H

#include "ScrollBarStrategy.h"
#include <vector>
#include <QScopedPointer>

class QScrollBar;
class YTextDocument;

class ExactScrollBarController : public ScrollBarStrategy
{
public:
    ExactScrollBarController(TailView *tailView);

    void onFileChanged();
    ScreenPosition position();
private:
    QScopedPointer<YTextDocument> m_document;
};

#endif // EXACTSCROLLBARCONTROLLER_H
