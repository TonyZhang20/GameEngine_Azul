#ifndef SPARESE_SET_H
#define SPARESE_SET_H

#include "ZVector.hpp"
#include <vector>


template <typename T, size_t PageSize = 4, typename = std::enable_if<std::is_integral_v<T>>>
//A hash map which is just for int size
class ZSparseSet final
{
public:
	inline void insert(T t)
	{
		density_.push_back(t);
		assure(t);
		index(t) = density_.size() - 1;
	}

	inline void remove(T t)
	{
		if (!contain(t)) return;
		T& idx = index(t);

		if (idx == density_.size() - 1)
		{
			idx = null;
			density_.pop_back();
		}
		else
		{
			T last = density_.back();
			index(last) = idx;
			std::swap(density_[idx], density_.back());
			idx = null;
			density_.pop_back();
		}
	}
	inline bool contain(T& t) const
	{
		assert(t != null);
		size_t p = page(t);
		size_t o = offset(t);

		return p < sparse_.size() && sparse_[p]->at(o) != null;
	}
	inline T find(T& t) const
	{
		if (contain(t));

		size_t p = page(t);
		size_t o = offset(t);

		return density_[sparse_[p]->at(o)];
	}
	inline void clear()
	{
		density_.clear();

		for (size_t i = 0; i < sparse_.size(); i++)
		{
			if (sparse_[i])
			{
				delete sparse_[i];
				sparse_[i] = nullptr;
			}
		}

		sparse_.clear();
	}

private:
	ZVector<T> density_;
	ZVector<ZVector<T>*> sparse_;
	~ZSparseSet() 
	{
		clear();
	}

private:
	static constexpr T null = std::numeric_limits<T>::max(); //获取类型极值
	inline size_t page(T& t) const
	{
		return t / PageSize;
	}
	inline T& index(T& t) const {

		return sparse_[page(t)]->at(offset(t));
	}
	inline size_t offset(T t) const {
		return t % PageSize;
	}
	inline void assure(T& t)
	{
		T p = page(t);

		if (p >= sparse_.size())
		{
			for (size_t i = sparse_.size(); i <= p; i++)
			{
				sparse_.push_back(new ZVector<T>(PageSize, null));
			}
		}
	}
};



#endif // !1
