/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 * 
 * Licensed under the GNU General Public License.  See license.txt for details.
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
