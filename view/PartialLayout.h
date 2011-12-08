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
class QTextBlock;

class PartialLayout: public LayoutStrategy {
public:
    PartialLayout(TailView * tailView);

    virtual bool onFileChanged(QString * error);
    virtual void resizeEvent();
    virtual int topScreenLine() const;
    virtual void scrollTo(int newTopLine);
    virtual bool scrollBy(int line_change);
    virtual void updateAfterKeyPress();
    virtual void vScrollBarAction(int action);
protected:
    virtual bool searchFile(bool isForward);
    virtual bool wrapAroundForDocumentSearch() const;
private:
    void updateScrollBars();
    bool updateView(qint64 new_line_address = -1, bool * at_bottom = 0);
    bool scrollUp(int line_change);
    bool scrollDown(int line_change);
    qint64 keepToLowerBound(const QTextBlock & block, int * blockLine);
    bool updateDocument(qint64 start_pos, qint64 lines_after_start);
    qint64 topOfScreen() const;

    void updateBottomDocument();
    qint64 bottomScreenPosition(int * blockLine = 0) const;
private:
    int m_topScreenLine;
    QScopedPointer<YTextDocument> m_bottomDocument;
    QScopedPointer<FileBlockReader> m_blockReader;
};

#endif // PARTIALLAYOUT_H
