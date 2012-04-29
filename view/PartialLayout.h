/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010-2012 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */

#ifndef PARTIALLAYOUT_H
#define PARTIALLAYOUT_H

#include <QScopedPointer>
#include <QtGlobal>

class FileBlockReader;
class TailView;
class YTextDocument;

class QTextBlock;

class PartialLayout {
public:
    PartialLayout(TailView * tailView);

    bool onFileChanged(QString * error);
    void performLayout();
    void resizeEvent();
    int topScreenLine() const;
    void scrollTo(int newTopLine);
    bool scrollBy(int line_change);
    void updateAfterKeyPress();
    void vScrollBarAction(int action);
    bool searchFile(bool isForward);
    bool wrapAroundForDocumentSearch() const;
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

    TailView * view();
    TailView * view() const;
    YTextDocument * document();
    YTextDocument * document() const;
private:
    int m_topScreenLine;
    QScopedPointer<YTextDocument> m_bottomDocument;
    QScopedPointer<FileBlockReader> m_blockReader;
    TailView * m_view;
};

#endif // PARTIALLAYOUT_H
