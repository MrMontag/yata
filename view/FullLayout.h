/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef FULLLAYOUT_H
#define FULLLAYOUT_H

#include "LayoutStrategy.h"

class FullLayout : public LayoutStrategy {
public:
    FullLayout(TailView * tailView);

    virtual bool onFileChanged(QString * error);
    virtual void performLayout();
    virtual void resizeEvent();
    virtual int topScreenLine() const;
    virtual void scrollTo(int newTopLine);
    virtual void updateAfterKeyPress();
    virtual void vScrollBarAction(int action);
    virtual bool searchFile(bool isForward);
    virtual bool wrapAroundForDocumentSearch() const;
};

#endif // FULLLAYOUT_H
