/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef PARTIALLAYOUT_H
#define PARTIALLAYOUT_H

#include "LayoutStrategy.h"

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
};

#endif // PARTIALLAYOUT_H
