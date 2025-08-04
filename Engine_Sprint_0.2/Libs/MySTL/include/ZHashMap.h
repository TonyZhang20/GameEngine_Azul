//#ifndef ZHASHMAP_H
//#define ZHASHMAP_H
//
//#include "ZVector.h"
//#include "GlobalNew.h"
//#include <stdexcept>
//
//template<typename Key, typename Val>
//class ZHashMap
//{
//private:
//    enum class State : uint8_t
//    {
//        EMPTY,
//        OCCUPIED,
//        DELETED
//    };
//
//    template<typename Key, typename Val>
//    struct Pair
//    {
//        Key key; 
//        Val value;
//        Pair* next;
//        Pair(const Key&k, const Val&v)
//            : key(k), value(v), next(nullptr)
//        {
//
//        }
//    };
//
//    using Bucket = Pair<Key, Val>;
//
//    template <typename Key>
//    size_t HashFunc(const Key& key) {
//        // hash to int
//        return static_cast<size_t>(key);
//    }
//
//    // hash to string
//    template <>
//    size_t HashFunc(const std::string& key) {
//        size_t hash = 0;
//        
//        for (char c : key) 
//        {
//            hash = hash * 31 + c; // 31 是质数，减少冲突
//        }
//
//        return hash;
//    }
//
//private:
//    ZVector<Bucket*> buckets;
//    //Azul::HeapNormal* mHeap;
//
//    size_t size; //current Size;
//    size_t bucket_count; //bucketsCount;
//    const float load_factor_threshold = 0.8f;
//    float padding;//just for padding
//
//private:
//    void rehash(size_t newCount)
//    {
//        if (newCount <= bucket_count)
//        {
//            assert(false);
//            return;
//        }
//
//        size_t newSize = newCount * sizeof(Bucket);
//
//        //Resize my Heap
//        //Azul::HeapNormal* reallocHeap;
//        //reallocHeap = Azul::Mem::ReAlloc(mHeap, newSize);
//        //if (reallocHeap == nullptr)
//        //{
//        //    assert(false);
//        //}
//        //mHeap = reallocHeap;
//
//        ZVector<Bucket*> newBuckets;
//        newBuckets.resize(newCount, nullptr);
//
//        Bucket* cur = nullptr;
//        Bucket* next = nullptr;
//
//        for (size_t i = 0; i < bucket_count; ++i)
//        {
//            cur = buckets[i];
//            while (cur)
//            {
//                next = cur->next;
//
//                size_t newIndex = HashFunc(cur->key) % newCount;
//
//                cur->next = newBuckets[newIndex];
//                newBuckets[newIndex] = cur;
//
//                cur = next;
//            }
//        }
//
//        this->bucket_count = newCount;
//
//    }
//
//public:
//
//    ZHashMap(uint32_t initBuckets = 16)
//        : 
//        size(0),
//        bucket_count(initBuckets)
//    {
//        buckets.resize(initBuckets, nullptr);
//
//        //Create Heap
//        //Azul::Mem::NormalHeap(this->mHeap, initBuckets * sizeof(Bucket), "Map Heap");
//    }
//
//    ~ZHashMap()
//    {
//        for (auto bucket : buckets)
//        {
//            Bucket* cur = bucket;
//            while (cur)
//            {
//                Bucket* next = cur->next; 
//                delete cur;
//                cur = next;
//            }
//        }
//    }
//
//    inline bool Insert(const Key& key, const Val& value)
//    {
//        //chek if need extend
//        if (size >= load_factor_threshold * bucket_count)
//        {
//            //Also Need to ReAllocHeap
//            rehash(bucket_count * 2);
//        }
//
//        size_t index = HashFunc(key) % bucket_count;
//
//        Bucket* cur = buckets[index];
//
//        while (cur) //find next slot
//        {
//            if (cur->key == key) 
//            {
//                return false;
//            }
//
//            cur = cur->next;
//        }
//
//        //Create Bucket Node
//        Bucket* newNode = new Bucket(key, value);
//
//        //Set Links -> intert from head
//        newNode->next = buckets[index];
//        buckets[index] = newNode;
//        
//        size++;
//        return true;
//    }
//
//    inline bool erase(const Key& key)
//    {
//        size_t index = HashFunc(key) % bucket_count;
//        Bucket* cur = buckets[index];
//        Bucket* prev = nullptr;
//
//        while (cur)
//        {
//            if (cur->key == key) 
//            {
//                if (prev == nullptr)
//                {
//                    buckets[index] = cur->next;
//                }
//                else
//                {
//                    prev->next = cur->next;
//                }
//                
//                delete cur;
//                size--;
//                return true;
//            }
//            
//            prev = cur;
//            cur = cur->next;
//        }
//
//        return false; //Find Nothing - Do Nothing
//    }
//
//};
//
//#endif