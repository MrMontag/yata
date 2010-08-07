#ifndef YFILECURSOR_H
#define YFILECURSOR_H

#include <QtGlobal>

struct YFileCursor {
    qint64 m_linePosition;
    int m_lineOffset;
    int m_length;

    YFileCursor(qint64 linePosition = -1, int lineOffset = -1, int length = -1)
        : m_linePosition(linePosition)
        , m_lineOffset(lineOffset)
        , m_length(length)
    {}

    bool isNull() const { return m_linePosition == -1; }
};

#endif // YFILECURSOR_H
