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
