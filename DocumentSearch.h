#ifndef DOCUMENTSEARCH_H
#define DOCUMENTSEARCH_H

#include <QScopedPointer>
#include <QString>
#include "BaseSearch.h"

class QTextDocument;
class QTextCursor;

class DocumentSearch: public BaseSearch {
public:
    DocumentSearch(QTextDocument * document);

    bool searchDocument(bool isForward, bool wrapAround = true);

    void setCursor(const QTextCursor & cursor);
    const QTextCursor & cursor() const;

private:
    void resetSearchCursor(bool isTop);
private:
    QTextDocument * m_document;

    QScopedPointer<QTextCursor> m_textCursor;

};

#endif // DOCUMENTSEARCH_H
