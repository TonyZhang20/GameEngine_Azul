#ifndef ZVECTOR_H
#define ZVECTOR_H

#include "GlobalNew.h"

template<typename T>
class ZVector
{
public:

    typedef T* iterator;
    typedef const T* const_iteraotr;

    inline ZVector();
    explicit inline ZVector(size_t count, const T& val = T(), const char* const Name = "Vector Heap");
    inline ~ZVector();

    inline ZVector(const ZVector<T>& other);

    inline ZVector<T>& operator = (const ZVector<T>& v)
    {
        if (this != &v)
        {
            ZVector tmp(v);
            swap(tmp);
        }

        return *this;
    }

    ZVector& operator=(ZVector&& other) noexcept
    {
        if (this != &other)
        {
            clear(); // 释放当前元素

            Azul::Mem::RemoveHeap(mHeap);

            _start = other._start;
            _finish = other._finish;
            _end_of_storage = other._end_of_storage;
            mHeap = other.mHeap;

            other._start = other._finish = other._end_of_storage = nullptr;
            other.mHeap = nullptr;
        }
        return *this;
    }

    //std move
    inline ZVector(ZVector&& other) noexcept
        : _start(other._start),
        _finish(other._finish),
        _end_of_storage(other._end_of_storage),
        mHeap(other.mHeap)
    {
        // 转移所有权后置空源对象
        other._start = other._finish = other._end_of_storage = nullptr;
        other.mHeap = nullptr;
    }

    inline void push_back(const T& value);
    inline void pop_back();
    inline void clear();

    //Create new vector, and copy data
    inline void reserve(size_t newcapacity)
    {
        ensure_capacity(newcapacity);
    }

    inline iterator insert(iterator pos, const T& value)
    {
        //if (index > m_size)
        //    throw std::out_of_range("insert index out of range");
        //ensure_capacity(m_size + 1);
        //for (size_t i = m_size; i > index; --i)
        //{
        //    m_data[i] = std::move(m_data[i - 1]);
        //}
        //m_data[index] = value;
        //++m_size;

        assert(pos <= _finish);

        if (_finish == _end_of_storage) //Full
        {
            size_t n = pos - _start;
            size_t  newCapcity = capacity() == 0 ? 2 : capacity() * 2;

            reserve(newCapcity);

            pos = _start + n;
        }

        iterator end = _finish - 1;
        while (end >= pos)
        {
            *(end + 1) = *(end);
            --end;
        }

        *pos = value;
        ++_finish;

        return pos;
    }
    inline void erase(iterator pos);

    inline void erase(const T& value)
    {
        for (auto it = begin(); it != end(); ++it)
        {
            if (*it == value)
            {
                erase(it);
                break;
            }
        }
    }

    inline size_t size() const;
    inline size_t capacity() const
    {
        return _end_of_storage - _start;
    };
    inline bool empty() const;

    inline T& operator[](size_t index);
    inline const T& operator[](size_t index) const;
    inline size_t operator()(const ZVector<T>& vec) const noexcept {} //不抛出异常 //Apply to hashmap

    template <typename T>
    bool operator==(const ZVector& other) const
    {
        return size() == other.size() &&
            std::equal(begin(), end(), other.begin());
    }

    inline T& at(size_t index)
    {
        assert(index < size() && "ZVector::operator[] - index out of bounds");
        return _start[index];
    }

    inline iterator begin() { return _start; };
    inline iterator end() { return _finish; };

    inline T& back() noexcept
    { 
        assert(size() > 0);

        T& data = _start[size() - 1]; 
        return data; 
    };
    inline const T& back() const noexcept
    {
        assert(size() > 0);

        T& data = _start[size() - 1];
        return data;
    }

    inline const_iteraotr begin() const { return _start; };
    inline const_iteraotr end() const { return _finish; };
    inline void resize(size_t count, const T& val = T());
    inline void ensure_capacity(size_t new_size);
    inline void swap(ZVector<T>& v) noexcept
    {
        std::swap(this->_start, v._start);
        std::swap(this->_finish, v._finish);
        std::swap(this->_end_of_storage, v._end_of_storage);

        std::swap(this->mHeap, v.mHeap);
    }

    template<typename... Args>
    inline void emplace_back(Args&&... args)
    {
        if (_finish == _end_of_storage)
        {
            size_t newcapacity = capacity() == 0 ? 2 : capacity() * 2;
            reserve(newcapacity);
        }

        new (_finish) T(std::forward<Args>(args)...);
        ++_finish;
    }

private:

    Azul::HeapNormal* mHeap;

    T* _start;
    T* _finish;
    T* _end_of_storage;
};

//Implementation
//Start with Size of 20
template<typename T>
inline ZVector<T>::ZVector()
{
    Azul::Mem::Create();

    Azul::Mem::Code memResult;
    memResult = Azul::Mem::NormalHeap(mHeap, sizeof(T) * 20 + 1024, "Vector Heap");

    _start = new(mHeap, Azul::Mem::Align::Byte_4, __FILE__, __LINE__) T[20];
    _finish = _start;
    _end_of_storage = _start + 20;
}

template<typename T>
inline ZVector<T>::ZVector(const ZVector<T>& v)
{
    Azul::Mem::Code memResult;
    memResult = Azul::Mem::NormalHeap(mHeap, v.capacity() * sizeof(T), "Vector Heap");
    assert(memResult == Azul::Mem::Code::OK);

    _start = new(mHeap, Azul::Mem::Align::Byte_4, __FILE__, __LINE__) T[v.capacity()];
    _finish = _start;
    _end_of_storage = _start + v.capacity();

    for (size_t i = 0; i < v.size(); ++i)
    {
        *_finish = v[i];
        ++_finish;
    }
}

template<typename T>
inline ZVector<T>::ZVector(size_t count, const T& val, const char* const Name)
{
    Azul::Mem::Code memResult;
    memResult = Azul::Mem::NormalHeap(mHeap, count * sizeof(T) + 1024, Name);
    assert(memResult == Azul::Mem::Code::OK);

    _start = new(mHeap, Azul::Mem::Align::Byte_4, __FILE__, __LINE__) T[count];
    _finish = _start + count;
    _end_of_storage = _start + count;

    for (size_t i = 0; i < size(); ++i)
    {
        _start[i] = val;
    }
}

template<typename T>
inline ZVector<T>::~ZVector()
{
    if (!this->mHeap) return;

    for (size_t i = 0; i < size(); i++)
    {
        _start[i].~T();
    }
    
    Azul::Mem::Code result;
    result = Azul::Mem::RemoveHeap(mHeap);

    if (result != Azul::Mem::Code::OK)
    {
        mHeap->CallDestructForAllBlock();
    }

    _start = nullptr;
    _finish = nullptr;
    _end_of_storage = nullptr;

    mHeap = nullptr;
}

template<typename T>
inline void ZVector<T>::push_back(const T& value)
{
    if (_finish == _end_of_storage)
    {
        size_t newcapacity = capacity() == 0 ? 2 : capacity() * 2;
        reserve(newcapacity);
    }

    *_finish = value;
    ++_finish;
}


template<typename T>
inline void ZVector<T>::pop_back()
{
    erase(end() - 1);
}

template<typename T>
inline void ZVector<T>::clear()
{
    if (this->mHeap == nullptr) return;
    for (auto it = begin(); it != end(); ++it)
    {
        it->~T();
    }
    _finish = _start;
}

template<typename T>
inline void ZVector<T>::erase(iterator pos)
{
    assert(pos >= _start && pos < _finish);

    if (pos + 1 != _finish)
    {
        std::move(pos + 1, _finish, pos);
    }

    (--_finish)->~T();
}

template<typename T>
inline size_t ZVector<T>::size() const { return _finish - _start; }


template<typename T>
inline bool ZVector<T>::empty() const { return size() == 0; }

template<typename T>
inline T& ZVector<T>::operator[](size_t index)
{
    assert(index < size() && "ZVector::operator[] - index out of bounds");
    return _start[index];
}

template<typename T>
inline const T& ZVector<T>::operator[](size_t index) const
{
    assert(index < size() && "ZVector::operator[] const - index out of bounds");
    return _start[index];
}

template<typename T>
inline void ZVector<T>::resize(size_t newSize, const T& val)
{
    size_t oldSize = size();

    if (newSize < oldSize)
    {
        for (size_t i = newSize; i < oldSize; ++i)
        {
            _start[i].~T();
        }

        _finish = _start + newSize;
    }
    else if (newSize > oldSize)
    {
        ensure_capacity(newSize);

        for (size_t i = oldSize; i < newSize; ++i)
        {
            if constexpr (std::is_pointer_v<T>) 
            {
                new(_start + i) T(nullptr);  // 指针初始化为nullptr
            }
            else 
            {
                new(_start + i) T(val);      // 非指针类型使用拷贝/移动
            }
        }

        _finish = _start + newSize;
    }
}

template<typename T>
inline void ZVector<T>::ensure_capacity(size_t new_cap)
{
    if (new_cap <= capacity()) return;

    size_t new_size = new_cap;
    size_t curSize = size();

    while (new_size < capacity())
        new_size *= 2;

    Azul::Heap* reallocHeap = Azul::Mem::ReAlloc(this->mHeap, new_size * sizeof(T) + 1024);

    if (reallocHeap == nullptr) //Realloc failed
    {
        Azul::Mem::Code memResult;
        Azul::HeapNormal* newHeap;

        memResult = Azul::Mem::NormalHeap(newHeap, new_size * sizeof(T) + 1024, mHeap->GetName());
        assert(memResult == Azul::Mem::Code::OK);

        T* new_data = new(newHeap, Azul::Mem::Align::Byte_4, __FILE__, __LINE__) T[new_size];

        for (size_t i = 0; i < curSize; ++i)
        {
            //*(new_data + i) = _start[i];
            new(new_data + i) T(std::move(_start[i]));
        }
        
        Azul::Mem::RemoveHeap(mHeap);

        mHeap = newHeap;

        _start = new_data;
        _finish = new_data + curSize;

        //m_data = new_data;
    }

    _finish = _start + curSize;
    this->_end_of_storage = _start + new_size;
}

#endif // !ZVECTOR_H