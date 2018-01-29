/*
 * Yata -- Yet Another Tail Application
 *
 * Copyright (C) 2010-2012 James Smith
 * Copyright (C) 2018  Alexander Fust
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
    const T & at(std::size_t i) const;
    const T & front() const;
    const T & back() const;

    std::size_t size() const;
    bool empty() const;
private:
    std::vector<T> m_data;
    QTextDocument * m_document;
};

#include "BlockDataVector.inl"

#endif // BLOCKDATAVECTOR_H
