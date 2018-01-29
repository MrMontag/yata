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
#ifndef FILESEARCH_H
#define FILESEARCH_H

#include "../search/BaseSearch.h"
#include "../document/YFileCursor.h"

class FileBlockReader;


class FileSearch: public BaseSearch {
public:
    FileSearch(const QString & filename);

    const YFileCursor & cursor() const;
    void setCursor(const YFileCursor & c);

    bool searchFile(bool isForward, bool wrapAround);

private:
    bool isMatch(const QRegExp & regex, YFileCursor * cursor, FileBlockReader * reader, int lineOffset, bool isForward);

private:
    QString m_filename;
    YFileCursor m_fileCursor;
};

#endif // FILESEARCH_H
