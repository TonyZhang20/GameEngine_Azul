#ifndef SPARESE_SET_H
#define SPARESE_SET_H

#include "ZVector.hpp"
#include "Entity.h"
#include <limits>

namespace zecs
{
	template <typename Component, size_t PageSize>
	//A hash map which is just for int size
	class ZSparseSet final
	{
	public:
		ZSparseSet()
			: entities(), components(), sparsePages()
		{

		}

		~ZSparseSet()
		{
			Clear();
		}

		Component& Add(EntityID id, Component&& component)//&& forward 完美转发
		{
			if (Contain(id))
			{
				uint32_t index = Index(id);
				Version(id) = id.version; //update version

				entities[index] = id;
				components[index] = std::forward<Component>(component);

				return components[index];
			}
			else
			{
				entities.push_back(id);
				components.emplace_back(std::forward<Component>(component));

				//setup Index --> index = push_back == size - 1
				Index(id) = entities.size() - 1;

				//setup version
				Version(id) = id.version;

				return components.back();
			}
		}

		Component& Get(EntityID& id)
		{
			uint32_t index = Index(id);
			return components[index];
		}

		void Remove(EntityID id)
		{
			if (!Contain(id)) return;
			if (Version(id) != id.version) return;

			//index what we want to Remove
			uint32_t& idx = Index(id);

			if (idx == entities.size() - 1) //last
			{
				entities.pop_back();
				components.pop_back();
			}
			else
			{
				EntityID last = entities.back();

				Index(last) = idx; // === index(last) = null
				
				//swap dense, swap target and last
				std::swap(entities[idx], entities.back());
				//update dense array, same as last 
				//同步变换 dense 数组
				components[idx] = std::move(components.back());

				entities.pop_back();
				components.pop_back();
			}

			idx = null; //change index(last)
			Version(id) = 0;

			// Current Page is empty, delete page
			if (IsPageEmpty(id.index / PageSize))
			{
				RemovePage(id.index / PageSize);
			}
		}

		bool Contain(EntityID id) const
		{
			size_t pageIndex = id.index / PageSize;
			size_t offset = id.index % PageSize;

			if (pageIndex >= sparsePages.size() || !sparsePages[pageIndex])
			{
				return false;
			}

			return sparsePages[pageIndex]->indices.at(offset) != null;
		}

		void Clear()
		{
			this->entities.clear();
			this->components.clear();

			size_t size = sparsePages.size();
			for (int i = 0; i < size; i++)
			{
				delete sparsePages[i];
			}

			this->sparsePages.clear();
		}

		//dense -> 用来检查重复
		ZVector<EntityID> entities;
		//dense either
		ZVector<Component> components;

		struct Page
		{
			std::array<uint8_t, PageSize> versions; // 存EntityID对应SparseSet中的版本
			std::array<uint32_t, PageSize> indices; // 在密度数组的索引

			Page() {}
		};

		ZVector<Page*> sparsePages;

	private:
		static constexpr uint32_t null = std::numeric_limits<uint32_t>::max();
		//Get Index From SparsePage
		uint32_t& Index(EntityID id)
		{
			size_t pageIndex = id.index / PageSize;
			size_t offset = id.index % PageSize;

			if (pageIndex >= sparsePages.size() || !sparsePages[pageIndex])
			{
				AllocatePage(pageIndex);
			}

			return sparsePages[pageIndex]->indices.at(offset);
		}

		uint8_t& Version(EntityID id)
		{
			size_t pageIndex = id.index / PageSize;
			size_t offset = id.index % PageSize;

			if (pageIndex >= sparsePages.size() || !sparsePages[pageIndex])
			{
				AllocatePage(pageIndex);
			}

			return sparsePages[pageIndex]->versions.at(offset);
		}

		void AllocatePage(size_t pageIndex)
		{
			if (pageIndex >= sparsePages.size())
			{
				sparsePages.resize(pageIndex + 1);
			}

			sparsePages[pageIndex] = new Page();

			Page* page = sparsePages[pageIndex];
			page->indices.fill(null);
			page->versions.fill(0);
		}

		void RemovePage(size_t pageIndex)
		{
			Page* page = sparsePages[pageIndex];
			
			sparsePages[pageIndex] = nullptr;
			
			delete page;
		}

		//查看某一Page是否为空
		bool IsPageEmpty(size_t pageIndex) const
		{
			if (pageIndex >= sparsePages.size() || !sparsePages[pageIndex])
			{
				return true;
			}

			Page* page = sparsePages[pageIndex];

			for (auto ind : page->indices)
			{
				if (ind != null)
				{
					return false;
				}
			}

			return true;
			//Equal to
			//return std::all_of(page->indices.begin(), page->indices.end(), [](uint32_t id) {
			//	return id == null; }
		}

	};
}





#endif // !1
