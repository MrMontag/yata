#include "BlockDataVector.h"
#include <algorithm>
#include <QTextBlock>
#include <QTextDocument>

template <class T>
BlockDataVector<T>::BlockDataVector(QTextDocument * doc):
    m_document(doc)
{
}

template <class T>
void BlockDataVector<T>::assign(const std::vector<T> & data)
{
    m_data = data;
}

template <class T>
void BlockDataVector<T>::push_back(const T & t)
{
    m_data.push_back(t);
}

template <class T>
void BlockDataVector<T>::clear()
{
    m_data.clear();
}

namespace {
    template <class T>
    struct FindBlockCmp { bool operator()(const T & x, const T & y) { return x > y; } };
}

template <class T>
QTextBlock BlockDataVector<T>::findContainingBlock(T item, T * closestItem) const
{
    // Find the value in m_data closest to item such that *blockitr <= item
    // (i.e., find the block that layoutLine is in)
    typename std::vector<T>::const_reverse_iterator itr = std::lower_bound(
        m_data.rbegin(),
        m_data.rend(),
        item,
        FindBlockCmp<T>());
    if(itr != m_data.rend()) {
        if(closestItem) {
            *closestItem = *itr;
        }
        int blockNumber = (m_data.rend() - itr) - 1;
        return m_document->findBlockByNumber(blockNumber);
    } else {
        return QTextBlock();
    }
}

template <class T>
const T & BlockDataVector<T>::at(size_t i) const
{
    return m_data.at(i);
}

template <class T>
const T & BlockDataVector<T>::front() const
{
    return m_data.front();
}

template <class T>
const T & BlockDataVector<T>::back() const
{
    return m_data.back();
}

template <class T>
size_t BlockDataVector<T>::size() const
{
    return m_data.size();
}

template <class T>
bool BlockDataVector<T>::empty() const
{
    return m_data.empty();
}
