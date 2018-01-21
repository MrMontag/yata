/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2013 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef SCROLLBARSTRATEGY_H
#define SCROLLBARSTRATEGY_H

class QScrollBar;
class TailView;
struct ScreenPosition;

class ScrollBarStrategy {
public:
    ScrollBarStrategy(TailView *tailView);
    virtual ~ScrollBarStrategy();

    virtual void onFileChanged() = 0;
    virtual ScreenPosition position() = 0;

protected:
    QScrollBar * scrollBar();
    TailView * view();

private:
    QScrollBar * m_scrollBar;
    TailView * m_view;
};

#endif // SCROLLBARSTRATEGY_H
