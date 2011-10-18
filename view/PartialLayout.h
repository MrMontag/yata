/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PARTIALLAYOUT_H
#define PARTIALLAYOUT_H

#include "LayoutStrategy.h"
#include <QScopedPointer>
#include <QtGlobal>

class YTextDocument;
class FileBlockReader;

class PartialLayout: public LayoutStrategy {
public:
    PartialLayout(TailView * tailView);

    virtual bool onFileChanged(QString * error);
    virtual void resizeEvent();
    virtual int topScreenLine() const;
    virtual void scrollTo(int newTopLine);
    virtual void updateAfterKeyPress();
    virtual void vScrollBarAction(int action);
protected:
    virtual bool searchFile(bool isForward);
    virtual bool wrapAroundForDocumentSearch() const;
private:
    void updateScrollBars();
    bool updateView(bool file_changed, qint64 new_line_address = -1, bool * needs_scroll = 0);
    bool scrollBy(int line_change);
    bool scrollUp(qint64 file_pos, int line_change);
    bool scrollDown(qint64 file_pos, int line_change, qint64 bottom_screen_pos);
    bool updateDocument(qint64 start_pos, qint64 lines_after_start);
    void updateBottomDocument();
    qint64 bottomScreenPosition() const;
private:
    int m_topScreenLine;
    int m_firstVisibleBlock;
    int m_firstVisibleBlockLine;
    QScopedPointer<YTextDocument> m_bottomDocument;
    QScopedPointer<FileBlockReader> m_blockReader;
};

#endif // PARTIALLAYOUT_H
