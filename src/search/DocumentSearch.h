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
#ifndef DOCUMENTSEARCH_H
#define DOCUMENTSEARCH_H

#include <QScopedPointer>
#include <QString>
#include "BaseSearch.h"

class YTextDocument;
class YFileCursor;

class DocumentSearch: public BaseSearch {
public:
    DocumentSearch(YTextDocument * document);

    ~DocumentSearch();

    bool searchDocument(bool isForward);

    void setCursor(const YFileCursor & cursor);
    const YFileCursor & cursor() const;

private:
    YTextDocument * m_document;

    QScopedPointer<YFileCursor> m_textCursor;
};

#endif // DOCUMENTSEARCH_H
