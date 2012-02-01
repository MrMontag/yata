/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#include "LayoutStrategy.h"

#include "TailView.h"
#include "YTextDocument.h"

LayoutStrategy::LayoutStrategy(TailView * tailView)
    : m_view(tailView)
{
}

LayoutStrategy::~LayoutStrategy()
{
}

void LayoutStrategy::performLayout()
{
    document()->layout(view()->viewport()->width());
}

TailView * LayoutStrategy::view()
{
    return m_view;
}

const TailView * LayoutStrategy::view() const
{
    return m_view;
}

YTextDocument * LayoutStrategy::document()
{
    return m_view->document();
}

const YTextDocument * LayoutStrategy::document() const
{
    return m_view->document();
}

