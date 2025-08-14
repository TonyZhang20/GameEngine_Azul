#ifndef ZHASHMAP_H
#define ZHASHMAP_H

#include "ZVector.hpp"
#include "GlobalNew.h"
#include <stdexcept>

template<typename Key, typename Val>
class ZHashMap
{
private:
    enum class State : uint8_t
    {
        EMPTY,
        OCCUPIED,
        DELETED
    };

    template<typename Key, typename Val>
    struct Pair
    {
        Key key;
        Val value;
        Pair* next;
        Pair(const Key& k, const Val& v)
            : key(k), value(v), next(nullptr)
        {

        }
    };

    using Bucket = Pair<Key, Val>;

    template <typename Key>
    size_t HashFunc(const Key& key) const {
        if constexpr (std::is_pointer_v<Key>) {
            return reinterpret_cast<size_t>(key);
        }
        else {
            return static_cast<size_t>(key);
        }
    }

    size_t HashFunc(const std::string& key) {
        size_t hash = 0;

        for (char c : key)
        {
            hash = hash * 31 + c; // 31 是质数，减少冲突
        }

        return hash;
    }

private:
    ZVector<Bucket*> buckets;
    //Azul::HeapNormal* mHeap;

    size_t element_count; //current Size;
    size_t bucket_count; //bucketsCount;
    const float load_factor_threshold = 1.f;
    float padding;//just for padding

private:
    void rehash(size_t newCount)
    {
        if (newCount <= bucket_count)
        {
            assert(false);
            return;
        }

        ZVector<Bucket*> newBuckets;
        newBuckets.resize(newCount, nullptr);

        Bucket* cur = nullptr;
        Bucket* next = nullptr;

        for (size_t i = 0; i < bucket_count; ++i)
        {
            cur = buckets[i];
            while (cur)
            {
                next = cur->next;

                size_t newIndex = HashFunc(cur->key) % newCount;

                cur->next = newBuckets[newIndex];
                newBuckets[newIndex] = cur;

                cur = next;
            }
        }

        buckets.swap(newBuckets);
        this->bucket_count = newCount;
    }

public:

    explicit ZHashMap(uint32_t initBuckets = 16)
        :
        element_count(0),
        bucket_count(next_prime(initBuckets))
    {
        buckets.resize(bucket_count, nullptr);

        //Create Heap
        //Azul::Mem::NormalHeap(this->mHeap, initBuckets * sizeof(Bucket), "Map Heap");
    }

    ~ZHashMap()
    {
        for (auto bucket : buckets)
        {
            Bucket* cur = bucket;
            while (cur)
            {
                Bucket* next = cur->next;

                delete cur;

                cur = next;
            }
        }
    }

    inline bool Insert(const Key& key, const Val& value)
    {
        //chek if need extend
        if (element_count >= load_factor_threshold * bucket_count)
        {
            //Also Need to ReAllocHeap
            rehash(next_prime(bucket_count * 2));
        }

        size_t index = HashFunc(key) % bucket_count;

        Bucket* cur = buckets[index];

        while (cur) //find next slot
        {
            if (cur->key == key)
            {
                return false;
            }

            cur = cur->next;
        }

        //Create Bucket Node
        Bucket* newNode = new Bucket(key, value);

        //Set Links -> intert from head
        newNode->next = buckets[index];
        buckets[index] = newNode;

        ++element_count;
        return true;
    }

    ZHashMap& operator= (const ZHashMap& other)
    {
        if (this != &other)
        {
            clear();
            buckets = std::move(other.buckets);
            element_count = other.element_count;
            bucket_count = other.bucket_count;
            load_factor_threshold = other.load_factor_threshold;
        }

        return *this;
    }

    //for std::move
    ZHashMap& operator= (ZHashMap&& other) noexcept
    {
        if (this != &other)
        {
            clear();

            buckets = std::move(other.buckets);
            element_count = other.element_count;
            bucket_count = other.bucket_count;
            load_factor_threshold = other.load_factor_threshold;

            other.element_count = 0;
            other.bucket_count = 0;
        }

        return *this;
    }

    inline void clear()
    {
        Bucket* curr = nullptr;
        Bucket* next = nullptr;

        for (size_t i = 0; i < bucket_count; ++i)
        {
            curr = buckets[i];
            while (curr)
            {
                next = curr->next;

                delete curr;

                curr = next;
            }
            buckets[i] = nullptr;
        }

        this->element_count = 0;
        this->bucket_count = 0;
    }

    inline float load_factor() const { return static_cast<float>(element_count / bucket_count); }

    Val* find(const Key& key)
    {
        size_t index = HashFunc(key) % bucket_count;
        Bucket* cur = buckets[index];

        while (cur != nullptr)
        {
            if (cur->key == key)
            {
                return &(cur->value);
            }

            cur = cur->next;
        }

        return nullptr;
    }

    const Val* find(const Key& key) const
    {
        size_t index = HashFunc(key) % bucket_count;
        Bucket* cur = buckets[index];

        while (cur != nullptr)
        {
            if (cur->key == key)
            {
                return &(cur->value);
            }

            cur = cur->next;
        }

        return nullptr;
    }

    Val& operator[](const Key& key)
    {
        Val* val_ptr = find(key);

        if (val_ptr == nullptr)
        {
            Insert(key, Val());
            val_ptr = find(key);
        }

        return *val_ptr;
    }

    inline bool erase(const Key& key)
    {
        size_t index = HashFunc(key) % bucket_count;
        Bucket* cur = buckets[index];
        Bucket* prev = nullptr;

        while (cur)
        {
            if (cur->key == key)
            {
                if (prev == nullptr)
                {
                    buckets[index] = cur->next;
                }
                else
                {
                    prev->next = cur->next;
                }

                delete cur;
                --element_count;
                return true;
            }

            prev = cur;
            cur = cur->next;
        }

        return false; //Find Nothing - Do Nothing
    }

    inline bool empty() const { return element_count == 0; }

    inline size_t size() const { return element_count; }
    inline size_t bucketcount() const {return bucket_count;}
    inline bool is_prime(size_t num) const {
        if (num <= 1) return false;
        if (num == 2) return true;
        if (num % 2 == 0) return false;
        for (size_t i = 3; i * i <= num; i += 2) {
            if (num % i == 0) return false;
        }
        return true;
    }

    inline size_t next_prime(size_t num) const {
        if (num <= 2) return 2;
        size_t candidate = num;
        if (candidate % 2 == 0) ++candidate;
        while (!is_prime(candidate)) {
            candidate += 2;
        }
        return candidate;
    }

    inline void reserve(size_t count)
    {
        size_t newCount = next_prime(count);
        if (newCount > bucket_count)
        {
            rehash(newCount);
        }
    }

};

#endif