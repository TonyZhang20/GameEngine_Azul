#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <type_traits>

namespace zecs
{
	namespace internal
	{
		template <typename Type>
		static constexpr int popcount(Type value) noexcept
		{
			return value ? (int(value & 1) + popcount(value >> 1)) : 0;
		}

		template <typename EntityT, typename = void>
		struct entity_traits;

		template <typename EntityT>
		struct entity_traits<EntityT, std::enable_if_t<std::is_enum_v<EntityT>>>
			: public entity_traits<std::underlying_type_t<EntityT>> {};

		//Enum to 32 bit / 16 bit
		template <>
		struct entity_traits<uint32_t> {
			using entity_type = uint32_t;

			using entity_mask_type = uint32_t;
			using version_mask_type = uint16_t;

			static constexpr entity_mask_type entity_mask = 0xFFFFF;
			static constexpr version_mask_type version_mask = 0xFFF;

			static constexpr entity_mask_type entity_mask_popcount = popcount(entity_mask);
		};

		//Enum to 64 bit / 32 bit
		template <>
		struct entity_traits<uint64_t> {
			using entity_type = uint64_t;

			using entity_mask_type = uint64_t;
			using version_mask_type = uint32_t;

			static constexpr entity_mask_type entity_mask = 0xFFFFFFFF;
			static constexpr version_mask_type version_mask = 0xFFFFFFFF;

			static constexpr entity_mask_type entity_mask_popcount = popcount(entity_mask);
		};

		//! @brief convert entity to it's underlying numeric type cast to int
		//! @tparam EntityT
		//! @param entity
		//! @return
		template <typename EntityT>
		constexpr auto entity_to_integral(EntityT entity) {
			using traits = entity_traits<EntityT>;
			return static_cast<typename traits::entity_type>(entity);
		}

		//! @brief get entity id number
		//! @tparam EntityT
		//! @param entity
		//! @return int and with mask
		template <typename EntityT>
		constexpr auto entity_id(EntityT entity) {
			using traits = entity_traits<EntityT>;
			return entity_to_integral(entity) & traits::entity_mask;
		}

		//! @brief get entity version number
		//! @tparam EntityT
		//! @param entity
		//! @return int right with version
		template <typename EntityT>
		constexpr auto entity_version(EntityT entity) {
			using traits = entity_traits<EntityT>;
			return entity_to_integral(entity) >> traits::entity_mask_popcount;
		}

		//! @brief return the next version of the entity
		//! @tparam EntityT
		//! @param entity
		//! @return add version/ 
		template <typename EntityT>
		constexpr auto entity_next_version(EntityT entity) {
			using traits = entity_traits<EntityT>;
			auto version = entity_to_integral(entity) >> traits::entity_mask_popcount;
			return version + 1 + (version == traits::version_mask); //ensure 11111 not exsits
		}

		//! @brief construct a entity by version and id
		//! @tparam EntityT
		//! @param version
		//! @param id
		//! @return 
		template <typename EntityT>
		constexpr EntityT construct_entity(
			typename entity_traits<EntityT>::version_mask_type version,
			typename entity_traits<EntityT>::entity_mask_type id) {
			using traits = entity_traits<EntityT>;
			return static_cast<EntityT>(
				((version & traits::version_mask) << traits::entity_mask_popcount) |
				(id & traits::entity_mask));
		}

		//! @brief combine two entities
		//! @tparam EntityT
		//! @param lhs pick the version of this entity
		//! @param rhs pick the id of this entity
		//! @return
		template <typename EntityT>
		constexpr EntityT combine_entity(EntityT lhs, EntityT rhs) {
			using traits = entity_traits<EntityT>;
			return static_cast<EntityT>(
				(entity_to_integral(lhs) &
					(traits::version_mask << traits::entity_mask_popcount)) |
				(entity_to_integral(rhs) & traits::entity_mask));
		}

		//! @brief increase the entity's version by 1 and return it
		//! @tparam EntityT
		//! @param entity
		//! @return entity
		template <typename EntityT>
		constexpr auto entity_inc_version(EntityT entity) {
			return construct_entity<EntityT>(entity_next_version(entity),
				entity_id(entity));
		}

		struct null_entity_t final { 
		public:
			template <typename EntityT>
			constexpr operator EntityT() const {
				using traits = entity_traits<EntityT>;
				return static_cast<EntityT>(
					traits::entity_mask |
					(traits::version_mask << traits::entity_mask_popcount));
			}

			constexpr bool operator==(null_entity_t) const { return true; }

			constexpr bool operator!=(null_entity_t) const { return false; }

			template <typename EntityT>
			constexpr bool operator==(EntityT entity) const {
				using traits = internal::entity_traits<EntityT>;
				return (traits::entity_mask & internal::entity_to_integral(entity)) ==
					traits::entity_mask;
			}

			template <typename EntityT>
			constexpr bool operator!=(EntityT entity) const {
				return !(*this == entity);
			}
		};

		template <typename EntityT>
		constexpr bool operator==(EntityT entity, null_entity_t null) {
			return null == entity;
		}

		template <typename EntityT>
		constexpr bool operator!=(EntityT entity, null_entity_t null) {
			return null != entity;
		}

		static constexpr null_entity_t null_entity = {};
	}

	constexpr auto& null_entity = internal::null_entity;

	/* c++ 17
	inline constexpr internal::null_entity_t null_entity = {};
	*/
}



#endif // !ENTITY_H
