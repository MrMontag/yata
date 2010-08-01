#include "BaseSearch.h"
#include <QRegExp>

BaseSearch::BaseSearch()
    : m_isSearchRegex(false)
    , m_isSearchCaseSensitive(false)
{
}

const QString & BaseSearch::lastSearchString() const
{
    return m_lastSearchString;
}

bool BaseSearch::searchWasRegex() const
{
    return m_isSearchRegex;
}

bool BaseSearch::searchWasCaseSensitive() const
{
    return m_isSearchCaseSensitive;
}

void BaseSearch::setSearchCriteria(const QString & searchString, bool isRegex, bool caseSensitive)
{
    m_lastSearchString = searchString;
    m_isSearchRegex = isRegex;
    m_isSearchCaseSensitive = caseSensitive;
}

QRegExp BaseSearch::getRegex() const
{
    return QRegExp(lastSearchString(),
        searchWasCaseSensitive() ? Qt::CaseSensitive : Qt::CaseInsensitive,
        searchWasRegex() ? QRegExp::RegExp2 : QRegExp::FixedString);
}
