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
#ifndef PARTIALLAYOUT_H
#define PARTIALLAYOUT_H

#include <QScopedPointer>
#include <QtGlobal>

class FileBlockReader;
struct ScreenPosition;
class TailView;
class YTextDocument;

class QTextBlock;

// TODO: remove the following line
#include "ScreenPosition.h"

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
private:
    bool updateView(ScreenPosition new_line_address, bool * at_bottom = 0);
    bool scrollUp(int line_change);
    bool scrollDown(int line_change);
    qint64 keepToLowerBound(const QTextBlock & block, int * blockLine);
    bool updateDocument(qint64 start_pos, qint64 lines_after_start);
    ScreenPosition topOfScreen() const;

    void updateBottomDocument();
    ScreenPosition bottomScreenPosition() const;

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
