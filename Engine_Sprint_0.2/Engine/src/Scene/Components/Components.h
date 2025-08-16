#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "MathEngine.h"
#include "Entity.h"

#define ErrorParent 0xffffff

namespace Azul
{
	struct TagComponent
	{
		char tag[32];

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const char* t)
		{
			strncpy_s(tag, sizeof(tag), t, _TRUNCATE);
		}

		void SetName(const char* t)
		{
			strncpy_s(tag, sizeof(tag), t, _TRUNCATE);
		}
	};

	struct TransformComponent
	{
		Vec3 scale = Vec3(1, 1, 1);
		//Quat rotation;
		Vec3 position;

		zecs::EntityID parent;

		TransformComponent()
		{
			parent.index = ErrorParent;
			parent.version = ErrorParent;
		}
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const Vec3& pos, Vec3& s)
			: position(pos), scale(s)
		{
		
		}

		inline Mat4 GetLocalMatrix() const
		{
			Trans TransA(position.x(), position.y(), position.z());
			Scale Scale(scale.x(), scale.y(), scale.z());

			return Scale * TransA;
		};
		inline Mat4 GetWorldMatrix() const
		{
			if (parent.index == ErrorParent)
			{
				return GetLocalMatrix();
			}
			else
			{
				//Parent.GetComponent<Transform>().GetWorldMatrix() * GetLocalMatrix();
				Trans TransA(position.x(), position.y(), position.z());
				Scale Scale(scale.x(), scale.y(), scale.z());

				return Scale * TransA;
			}
		};
	};

	struct SpriteRendererComponent
	{
		Vec4 Color {1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vec4& color) : Color(color) {}
	};
}

#endif // !TRANSFORM_H
