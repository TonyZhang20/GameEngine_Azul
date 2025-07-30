#ifndef VECTOR_H
#define VECTOR_H

#include "MySTLDLLInterface.h"

namespace Azul
{
    template<typename T>
    class MYSTL_LIBRARY_API Vector {
    public:
        using iterator = T*;
        using const_iterator = const T*;

        Vector();
        Vector(size_t count);
        Vector(std::initializer_list<T> init);
        ~Vector();

        Vector(const Vector& other);
        Vector(Vector&& other) noexcept;
        Vector& operator=(Vector other);

        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();
        void clear();
        void insert(size_t index, const T& value);
        void erase(size_t index);

        size_t size() const;
        size_t capacity() const;
        bool empty() const;

        T& operator[](size_t index);
        const T& operator[](size_t index) const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

    private:
        void ensure_capacity(size_t new_size);

        T* m_data;
        size_t m_size;
        size_t m_capacity;
    };

    // Implementation

    template<typename T>
    Vector<T>::Vector()
        : m_data(nullptr), m_size(0), m_capacity(0) {}

    template<typename T>
    Vector<T>::Vector(size_t count)
        : m_data(new T[count]()), m_size(count), m_capacity(count) {}

    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> init)
        : Vector(init.size()) {
        size_t i = 0;
        for (const auto& val : init)
            m_data[i++] = val;
    }

    template<typename T>
    Vector<T>::~Vector() {
        delete[] m_data;
    }

    template<typename T>
    Vector<T>::Vector(const Vector& other)
        : m_data(new T[other.m_capacity]), m_size(other.m_size), m_capacity(other.m_capacity) {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = other.m_data[i];
    }

    template<typename T>
    Vector<T>::Vector(Vector&& other) noexcept
        : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_capacity = 0;
    }

    template<typename T>
    Vector<T>& Vector<T>::operator=(Vector other) {
        swap(*this, other);
        return *this;
    }

    template<typename T>
    void Vector<T>::push_back(const T& value) {
        ensure_capacity(m_size + 1);
        m_data[m_size++] = value;
    }

    template<typename T>
    void Vector<T>::push_back(T&& value) {
        ensure_capacity(m_size + 1);
        m_data[m_size++] = std::move(value);
    }

    template<typename T>
    void Vector<T>::pop_back() {
        if (m_size == 0)
            throw std::out_of_range("pop_back on empty vector");
        --m_size;
    }

    template<typename T>
    void Vector<T>::clear() {
        for (size_t i = 0; i < m_size; ++i)
            m_data[i].~T();
        m_size = 0;
    }

    template<typename T>
    void Vector<T>::insert(size_t index, const T& value) {
        if (index > m_size)
            throw std::out_of_range("insert index out of range");
        ensure_capacity(m_size + 1);
        for (size_t i = m_size; i > index; --i)
            m_data[i] = std::move(m_data[i - 1]);
        m_data[index] = value;
        ++m_size;
    }

    template<typename T>
    void Vector<T>::erase(size_t index) {
        if (index >= m_size)
            throw std::out_of_range("erase index out of range");
        for (size_t i = index; i < m_size - 1; ++i)
            m_data[i] = std::move(m_data[i + 1]);
        --m_size;
    }

    template<typename T>
    size_t Vector<T>::size() const { return m_size; }

    template<typename T>
    size_t Vector<T>::capacity() const { return m_capacity; }

    template<typename T>
    bool Vector<T>::empty() const { return m_size == 0; }

    template<typename T>
    T& Vector<T>::operator[](size_t index) {
        return m_data[index];
    }

    template<typename T>
    const T& Vector<T>::operator[](size_t index) const {
        return m_data[index];
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::begin() {
        return m_data;
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::end() {
        return m_data + m_size;
    }

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::begin() const {
        return m_data;
    }

    template<typename T>
    typename Vector<T>::const_iterator Vector<T>::end() const {
        return m_data + m_size;
    }


    template<typename T>
    void Vector<T>::ensure_capacity(size_t new_size) {
        if (new_size <= m_capacity)
            return;
        size_t new_capacity = m_capacity == 0 ? 1 : m_capacity * 2;
        while (new_capacity < new_size)
            new_capacity *= 2;

        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < m_size; ++i)
            new_data[i] = std::move(m_data[i]);
        delete[] m_data;
        m_data = new_data;
        m_capacity = new_capacity;
    }
};


#endif // !VECTOR_H
