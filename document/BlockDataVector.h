#ifndef BLOCKDATAVECTOR_H
#define BLOCKDATAVECTOR_H

#include <vector>

class QTextBlock;
class QTextDocument;

template <class T>
class BlockDataVector {
public:
    BlockDataVector(QTextDocument * doc);
    void assign(const std::vector<T> & data);
    void push_back(const T & t);
    void clear();
    QTextBlock findContainingBlock(T item, T * closestItem = 0) const;
    T at(std::size_t i) const;
    std::size_t size() const;
    bool empty() const;
private:
    std::vector<T> m_data;
    QTextDocument * m_document;
};

#include "BlockDataVector.inl"

#endif // BLOCKDATAVECTOR_H
