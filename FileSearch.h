#ifndef FILESEARCH_H
#define FILESEARCH_H

#include "BaseSearch.h"
#include "YFileCursor.h"

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
