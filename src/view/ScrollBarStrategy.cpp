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
#include "ScrollBarStrategy.h"
#include "TailView.h"
#include <QScrollBar>

ScrollBarStrategy::ScrollBarStrategy(TailView * tailView):
    m_view(tailView)
{
    m_scrollBar = tailView->verticalScrollBar();
}

ScrollBarStrategy::~ScrollBarStrategy()
{
}

QScrollBar * ScrollBarStrategy::scrollBar()
{
    return m_scrollBar;
}

TailView * ScrollBarStrategy::view()
{
    return m_view;
}

//void ScrollBarStrategy::vScrollBarAction(int action)
//{
//    if(action == QAbstractSlider::SliderMove && !m_scrollBar->isSliderDown()) {
//        // The slider moved, but wasn't by a keystroke (since isSliderDown() returned false),
//        // so assume a wheel event triggered this action (which gets handled by wheelEvent()),
//        // and simply return.
//        return;
//    }
//
//    if(m_scrollBar->sliderPosition() > m_scrollBar->maximum()) {
//        m_scrollBar->setSliderPosition(m_scrollBar->maximum());
//    }
//
//    if(m_scrollBar->sliderPosition() < m_scrollBar->minimum()) {
//        m_scrollBar->setSliderPosition(m_scrollBar->minimum());
//    }
//
//    int line_change = 0;
//    int page_step = view()->numLinesOnScreen() - PAGE_STEP_OVERLAP;
//
//    switch(action) {
//    case QAbstractSlider::SliderSingleStepAdd:
//        line_change = 1;
//        break;
//    case QAbstractSlider::SliderSingleStepSub:
//        line_change = -1;
//        break;
//    case QAbstractSlider::SliderPageStepAdd:
//        line_change = page_step;
//        break;
//    case QAbstractSlider::SliderPageStepSub:
//        line_change = -page_step;
//        break;
//    default:
//        break;
//    }
//
//    strategyAction(line_change);
//}
