/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef PARTIALLAYOUT_H
#define PARTIALLAYOUT_H

#include "LayoutStrategy.h"
#include <QtGlobal>

class PartialLayout : public LayoutStrategy
{
public:
    PartialLayout(TailView * tailView);

    virtual void onFileChanged();
    virtual void resizeEvent();
    virtual int topScreenLine() const;
    virtual void scrollTo(int newTopLine);
    virtual void updateAfterKeyPress();
    virtual void vScrollBarAction(int action);
protected:
    virtual bool searchFile(bool isForward);
    virtual bool wrapAroundForDocumentSearch() const;
private:
    void updateDocumentForPartialLayout(
        bool file_changed = false, int line_change = 0, qint64 new_line_address = -1);
    void updateDocument(qint64 start_pos, qint64 lines_after_start, qint64 num_lines);
private:
    int m_firstVisibleLayoutLine;
    int m_firstVisibleBlock;
    int m_firstVisibleBlockLine;
    qint64 m_lastFileAddress;
};

#endif // PARTIALLAYOUT_H
