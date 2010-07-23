#ifndef DOCUMENTSEARCH_H
#define DOCUMENTSEARCH_H

#include <QScopedPointer>
#include <QString>

class QTextDocument;
class QTextCursor;

class DocumentSearch {
public:
    DocumentSearch(QTextDocument * document);

    const QString & lastSearchString() const;
    bool searchWasRegex() const;
    bool searchWasCaseSensitive() const;
    void setSearchCriteria(const QString & searchString, bool isRegex, bool caseSensitive);
    bool searchDocument(bool isForward, bool wrapAround = true);

    void setCursor(const QTextCursor & cursor);
    const QTextCursor & cursor() const;

private:
    void resetSearchCursor(bool isTop);
private:
    QTextDocument * m_document;

    QString m_lastSearchString;
    bool m_isSearchRegex;
    bool m_isSearchCaseSensitive;
    QScopedPointer<QTextCursor> m_textCursor;

};

#endif // DOCUMENTSEARCH_H
