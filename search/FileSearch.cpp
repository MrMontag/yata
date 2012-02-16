/*
 * This file is part of yata -- Yet Another Tail Application
 * Copyright 2010 James Smith
 *
 * Licensed under the GNU General Public License.  See license.txt for details.
 */
#include "FileSearch.h"
#include "FileBlockReader.h"

#include <QRegExp>

FileSearch::FileSearch(const QString & filename):
    m_filename(filename)
{
}

const YFileCursor & FileSearch::cursor() const
{
    return m_fileCursor;
}

void FileSearch::setCursor(const YFileCursor & c)
{
    m_fileCursor = c;
}

bool FileSearch::searchFile(bool isForward, bool wrapAround)
{
    if(lastSearchString().isEmpty()) {
        m_fileCursor.makeNull();
        return false;
    }

    if(m_fileCursor.isNull()) {
        m_fileCursor = YFileCursor(0);
    }

    qint64 initialLinePosition = m_fileCursor.lineAddress();

    QRegExp regex(getRegex());
    FileBlockReader reader(m_filename);

    int lineOffset = 0;

    bool initialLoop = true;

    while(true) {
        if(isMatch(regex, &m_fileCursor, &reader, lineOffset, isForward)) {
            return true;
        }

        if(isForward) {
            lineOffset = 1;
            m_fileCursor.setCharPos(0);
            if(m_fileCursor.lineAddress() >= reader.size() - 1) {
                if(!wrapAround) { return false; }
                m_fileCursor.setLineAddress(0);
                lineOffset = 0;
            }
        } else {
            lineOffset = -1;
            m_fileCursor.setCharPos(YFileCursor::NULL_VALUE);
            if(m_fileCursor.lineAddress() == 0) {
                if(!wrapAround) { return false; }
                m_fileCursor.setLineAddress(reader.size());
                lineOffset = 0;
            }
        }

        if(!initialLoop && m_fileCursor.lineAddress() == initialLinePosition) {
            return false;
        }

        initialLoop = false;
    }
}

bool FileSearch::isMatch(const QRegExp & regex, YFileCursor * cursor, FileBlockReader * reader, int lineOffset, bool isForward)
{
    std::vector<qint64> linePositions;
    QString data;

    reader->readChunk(&data, &linePositions, cursor->lineAddress(), lineOffset, 1);

    if(cursor->charPos() == YFileCursor::NULL_VALUE) {
        cursor->setCharPos(data.length());
    }

    if(linePositions.empty()) {
        return false;
    }

    int startPos = -1;
    int matchLength = 0;
    if(isForward) {
        startPos = regex.indexIn(data, cursor->charPos() + cursor->length());
        matchLength = regex.matchedLength();
    } else {
        int pos = 0;
        while(true) {
            pos = regex.indexIn(data, pos + matchLength);
            if(pos == -1) {
                break;
            }

            if(pos + regex.matchedLength() < cursor->charPos()) {
                startPos = pos;
                matchLength = regex.matchedLength();
            } else {
                break;
            }
        }
    }

    cursor->setLineAddress(linePositions.front());

    if(startPos != -1) {
        cursor->setCharPos(startPos);
        cursor->setLength(matchLength);
        return true;
    } else {
        cursor->setCharPos(YFileCursor::NULL_VALUE);
        cursor->setLength(0);
        return false;
    }
}
