/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FullLayout.h"

#include "FileBlockReader.h"
#include "TailView.h"
#include "YTextDocument.h"

#include <QScrollBar>

FullLayout::FullLayout(TailView * view)
    : LayoutStrategy(view)
{
}

void FullLayout::onFileChanged()
{
    QString data;
    view()->m_blockReader->readAll(&data, &view()->m_lineAddresses);
    view()->setDocumentText(data);
    if(view()->followTail()) {
        performLayout();
        view()->verticalScrollBar()->setValue(view()->verticalScrollBar()->maximum());
    }
}

void FullLayout::performLayout()
{
    LayoutStrategy::performLayout();
    view()->updateScrollBars(view()->m_document->numLayoutLines());
}

void FullLayout::resizeEvent()
{
    view()->updateScrollBars(view()->m_document->numLayoutLines());
}

int FullLayout::topScreenLine() const
{
    return view()->verticalScrollBar()->sliderPosition();
}

void FullLayout::scrollTo(int newTopLine)
{
    view()->verticalScrollBar()->setValue(newTopLine);
}

bool FullLayout::searchFile(bool /*isForward*/)
{
    // Called when searching the document failed.
    // In full layout, the file has the same contents,
    // so just return false.
    return false;
}

void FullLayout::updateAfterKeyPress()
{
}

bool FullLayout::wrapAroundForDocumentSearch() const
{
    return true;
}
