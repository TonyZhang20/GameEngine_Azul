#ifndef QUEUE_H
#define QUEUE_H


#include "Vector.h"
#include <stdexcept>

namespace Simple {

    template<typename T>
    class Queue {
    public:
        Queue() = default;

        void enqueue(const T& value) {
            m_data.push_back(value);
        }

        void enqueue(T&& value) {
            m_data.push_back(std::move(value));
        }

        void dequeue() {
            if (empty())
                throw std::out_of_range("dequeue on empty queue");
            m_data.erase(0);
        }

        T& front() {
            if (empty())
                throw std::out_of_range("front on empty queue");
            return m_data[0];
        }

        const T& front() const {
            if (empty())
                throw std::out_of_range("front on empty queue");
            return m_data[0];
        }

        size_t size() const {
            return m_data.size();
        }

        bool empty() const {
            return m_data.empty();
        }

        void clear() {
            m_data.clear();
        }

    private:
        Vector<T> m_data;
    };

}

#endif // !QUEUE_H
