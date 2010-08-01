#ifndef FILESEARCH_H
#define FILESEARCH_H

#include <QtGlobal>
#include "BaseSearch.h"

class FileBlockReader;

struct FileCursor {
    qint64 m_linePosition;
    int m_lineOffset;
    int m_length;

    FileCursor(qint64 linePosition = -1, int lineOffset = -1, int length = -1)
        : m_linePosition(linePosition)
        , m_lineOffset(lineOffset)
        , m_length(length)
    {}

    bool isNull() const { return m_linePosition == -1; }
};

class FileSearch: public BaseSearch {
public:
    FileSearch(const QString & filename);

    const FileCursor & cursor() const;
    void setCursor(const FileCursor & c);

    bool searchFile(bool isForward, bool wrapAround);

private:
    bool isMatch(const QRegExp & regex, FileCursor * cursor, FileBlockReader * reader, int lineOffset, bool isForward);

private:
    QString m_filename;
    FileCursor m_fileCursor;
};

#endif // FILESEARCH_H
