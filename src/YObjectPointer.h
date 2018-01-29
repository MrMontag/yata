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
#ifndef YOBJECTPOINTER_H
#define YOBJECTPOINTER_H

#include <QPointer>

// Note: T must be a QObject or a subclass of QObject.
template <typename T> class YObjectPointer {
public:
    explicit YObjectPointer(T * t = 0);
    ~YObjectPointer();
    T * data() const;
    void reset(T * other = 0);
    T * take();
    operator bool() const;
    bool operator!() const;
    T & operator*() const;
    T * operator->() const;

private:
    YObjectPointer(YObjectPointer &);
    YObjectPointer<T> & operator=(YObjectPointer<T> &);

private:
    QPointer<T> m_ptr;
};

template <typename T> YObjectPointer<T>::YObjectPointer(T * t):
    m_ptr(t)
{
}

template <typename T> YObjectPointer<T>::~YObjectPointer()
{
    delete m_ptr.data();
}

template <typename T> T * YObjectPointer<T>::data() const
{
    return m_ptr.data();
}

template <typename T> void YObjectPointer<T>::reset(T * other)
{
    if(m_ptr.data() == other) { return; }

    delete m_ptr.data();
    m_ptr = other;
}

template <typename T> T * YObjectPointer<T>::take()
{
    T * t = m_ptr.data();
    m_ptr = 0;
    return t;
}

template <typename T> YObjectPointer<T>::operator bool() const
{
    return m_ptr;
}

template <typename T> bool YObjectPointer<T>::operator!() const
{
    return !m_ptr;
}

template <typename T> T & YObjectPointer<T>::operator*() const
{
    return *m_ptr.data();
}

template <typename T> T * YObjectPointer<T>::operator->() const
{
    return m_ptr.data();
}

#endif // YOBJECTPOINTER_H
