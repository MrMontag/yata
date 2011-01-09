/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#ifndef LAYOUTSTRATEGY_H
#define LAYOUTSTRATEGY_H

class TailView;

class LayoutStrategy
{
public:
    virtual ~LayoutStrategy();

    void search(bool isForward);
    virtual void onFileChanged() = 0;
    virtual void performLayout();
    virtual void resizeEvent() = 0;
    virtual int topScreenLine() const = 0;
    virtual void scrollTo(int newTopLine) = 0;
    virtual void updateAfterKeyPress() = 0;
protected:
    LayoutStrategy(TailView * tailView);

    virtual bool searchFile(bool isForward) = 0;
    virtual bool wrapAroundForDocumentSearch() const = 0;

    TailView * view();
    const TailView * view() const;
private:
    TailView * m_view;
};

#endif // LAYOUTSTRATEGY_H
