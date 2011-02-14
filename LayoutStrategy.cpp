/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "LayoutStrategy.h"

#include "DocumentSearch.h"
#include "TailView.h"
#include "YTextDocument.h"
#include "YApplication.h"

#include <QMessageBox>
#include <QScrollBar>
#include <QTextStream>

LayoutStrategy::LayoutStrategy(TailView * tailView)
    : m_view(tailView)
{
}

LayoutStrategy::~LayoutStrategy()
{
}

void LayoutStrategy::performLayout()
{
    view()->document()->layout(view()->viewport()->width());
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

