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
#include "ApproximateScrollBarController.h"
#include "TailView.h"
#include "PartialLayout.h"

#include <QScrollBar>

const qint64 APPROXIMATE_CHARS_PER_LINE = 20;

ApproximateScrollBarController::ApproximateScrollBarController(TailView * view):
	ScrollBarStrategy(view),
	m_lastSBPosition(-1)
{
}
    
void ApproximateScrollBarController::onFileChanged()
{
}

ScreenPosition ApproximateScrollBarController::position()
{
    QScrollBar * verticalScrollBar = view()->verticalScrollBar();

    if (verticalScrollBar->sliderPosition() != m_lastSBPosition)
    {
        m_lastSBPosition = verticalScrollBar->sliderPosition();
        if(verticalScrollBar->sliderPosition() >= verticalScrollBar->maximum()) {
            m_currentScreenPos = ScreenPosition::MAX;
        } else {
            m_currentScreenPos.address = static_cast<qint64>(verticalScrollBar->sliderPosition()) * APPROXIMATE_CHARS_PER_LINE;
            m_currentScreenPos.blockLine = 0;
        }
    }

    return m_currentScreenPos;
}
