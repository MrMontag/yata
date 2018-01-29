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
#include "ExactScrollBarController.h"
#include "fileio/FileBlockReader.h"
#include "view/PartialLayout.h"
#include "view/TailView.h"
#include "document/YTextDocument.h"

#include <QScrollBar>

ExactScrollBarController::ExactScrollBarController(TailView *tailView):
    ScrollBarStrategy(tailView),
    m_document(new YTextDocument())
{
}

void ExactScrollBarController::onFileChanged()
{
    QString data;
    std::vector<qint64> lineAddresses;
    FileBlockReader blockReader(view()->filename());
    /*bool success =*/ blockReader.readAll(&data, &lineAddresses);
    m_document->setText(data, lineAddresses);
    m_document->layout(view()->width());
    view()->updateScrollBars(m_document->numLayoutLines() - view()->numLinesOnScreen());
    //if(view()->followTail()) {
    //    scrollBar()->setValue(scrollBar()->maximum());
    //}

    //if(!success) { *error = blockReader.errorString(); }

    //return success;
}

ScreenPosition ExactScrollBarController::position()
{
    int sbpos = scrollBar()->sliderPosition();
    int topOfBlock = 0;
    QTextBlock block = m_document->blockLayoutLines().findContainingBlock(sbpos, &topOfBlock);
    qint64 addr = m_document->blockAddress(block);
    ScreenPosition pos(addr, sbpos - topOfBlock);
    return pos;
}
