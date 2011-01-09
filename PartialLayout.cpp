/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "PartialLayout.h"

#include "DocumentSearch.h"
#include "FileSearch.h"
#include "FileBlockReader.h"
#include "TailView.h"

PartialLayout::PartialLayout(TailView * tailView)
    : LayoutStrategy(tailView)
{
}

void PartialLayout::onFileChanged()
{
    view()->updateDocumentForPartialLayout(true);
}

void PartialLayout::resizeEvent()
{
    view()->updateDocumentForPartialLayout();
}

int PartialLayout::topScreenLine() const
{
    return view()->m_firstVisibleLayoutLine;
}

void PartialLayout::scrollTo(int newTopLine)
{
    int lineChange = newTopLine - topScreenLine();
    view()->updateDocumentForPartialLayout(false, lineChange);
}

bool PartialLayout::searchFile(bool isForward)
{
    bool matchFound = false;
    FileSearch fileSearch(view()->m_filename);
    fileSearch.setCursor(*view()->m_fileCursor);
    fileSearch.setSearchCriteria(
        view()->m_documentSearch->lastSearchString(),
        view()->m_documentSearch->searchWasRegex(),
        view()->m_documentSearch->searchWasCaseSensitive());
    if(fileSearch.searchFile(isForward, true)) {
        matchFound = true;
        YFileCursor cursor(fileSearch.cursor());
        *view()->m_fileCursor = cursor;

        int line_change = -(view()->numLinesOnScreen()/2);
        view()->updateDocumentForPartialLayout(false, line_change, view()->m_fileCursor->m_lineAddress);
    }

    return matchFound;
}

bool PartialLayout::wrapAroundForDocumentSearch() const
{
    return false;
}

void PartialLayout::updateAfterKeyPress()
{
    view()->updateDocumentForPartialLayout();
    view()->viewport()->update();
}

