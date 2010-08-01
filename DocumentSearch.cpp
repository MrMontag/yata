#include "DocumentSearch.h"
#include <QTextCursor>
#include <QTextDocument>
#include <QApplication>
#include <QPalette>

DocumentSearch::DocumentSearch(QTextDocument * document)
    : m_document(document)
    , m_textCursor(new QTextCursor)
{
}

void DocumentSearch::setCursor(const QTextCursor & cursor)
{
    QTextCharFormat clear;
    m_textCursor->setCharFormat(clear);
    *m_textCursor = cursor;
    highlightCursor();
}

const QTextCursor & DocumentSearch::cursor() const
{
    return *m_textCursor;
}

void DocumentSearch::resetSearchCursor(bool isTop)
{
    *m_textCursor = QTextCursor(m_document);
    m_textCursor->movePosition(isTop ? QTextCursor::Start : QTextCursor::End);
}

void DocumentSearch::highlightCursor()
{
    QTextCharFormat format;

    // TODO: make the palette customizable (for now use the system palette)
    QPalette palette = QApplication::palette();
    format.setBackground(palette.highlight());
    format.setForeground(palette.highlightedText());
    m_textCursor->setCharFormat(format);
}

bool DocumentSearch::searchDocument(bool isForward, bool wrapAround)
{
    if(m_textCursor->isNull()) {
        *m_textCursor = QTextCursor(m_document);
    } else {
        QTextCharFormat clear;
        m_textCursor->setCharFormat(clear);
    }

    QRegExp regex(getRegex());

    QTextDocument::FindFlags flags = 0;
    if(!isForward) {
        flags |= QTextDocument::FindBackward;
    }

    QTextCursor match = m_document->find(regex, *m_textCursor, flags);

    if(wrapAround && match.isNull()) {
        resetSearchCursor(isForward);
        match = m_document->find(regex, *m_textCursor, flags);
    }

    if(!match.isNull()) {
        *m_textCursor = match;
        highlightCursor();
    }

    return !match.isNull();
}
