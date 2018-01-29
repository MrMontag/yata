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
#include "ScreenPosition.h"

ScreenPosition::ScreenPosition(qint64 a, int bl):
    address(a),
    blockLine(bl)
{
}

bool ScreenPosition::operator==(const ScreenPosition & other) const
{
    return address == other.address && blockLine == other.blockLine;
}

const ScreenPosition ScreenPosition::MAX = ScreenPosition(-2, -2);
