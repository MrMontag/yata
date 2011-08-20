/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2011 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PARTIALLAYOUT_H
#define PARTIALLAYOUT_H

#include "LayoutStrategy.h"
#include <QtGlobal>
#include <memory>

class YTextDocument;
class FileBlockReader;

class PartialLayout: public LayoutStrategy {
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
    void updateBottomDocument();
    qint64 bottomScreenPosition() const;
private:
    int m_topScreenLine;
    int m_firstVisibleBlock;
    int m_firstVisibleBlockLine;
    qint64 m_lastFileAddress;
    std::auto_ptr<YTextDocument> m_bottomDocument;
    std::auto_ptr<FileBlockReader> m_blockReader;
};

#endif // PARTIALLAYOUT_H
