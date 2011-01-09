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
#include <QTextStream>

LayoutStrategy::LayoutStrategy(TailView * tailView)
    : m_view(tailView)
{
}

LayoutStrategy::~LayoutStrategy()
{
}

void LayoutStrategy::search(bool isForward)
{
    bool wrapAround = wrapAroundForDocumentSearch();
    bool matchFound = view()->searchDocument(isForward, wrapAround);
    if(matchFound) {
        view()->scrollToIfNecessary(view()->m_documentSearch->cursor());
    } else {
        matchFound = searchFile(isForward);
    }

    if(!matchFound) {
        QString message;
        QTextStream(&message)
            << QObject::tr("Pattern \"")
            << view()->m_documentSearch->lastSearchString()
            << QObject::tr("\" not found");
        QMessageBox::information(view(), YApplication::displayAppName(), message);
    }

    view()->viewport()->update();
}

void LayoutStrategy::performLayout()
{
    view()->m_document->layout(view()->viewport()->width());
}

TailView * LayoutStrategy::view()
{
    return m_view;
}

const TailView * LayoutStrategy::view() const
{
    return m_view;
}

