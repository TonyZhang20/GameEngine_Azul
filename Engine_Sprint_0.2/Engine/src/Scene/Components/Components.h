#ifndef COMPONENT_H
#define COMPONENT_H

#include "MathEngine.h"
#include "Entity.h"
#include <d3d11.h>
#include "DirectXTex.h"

#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderObject.h"
#include "TextureObject.h"

#define ErrorParent 0xfffff

namespace Azul
{
	enum class RasterizerStateID
	{
		D3D11_FILL_SOLID,
		D3D11_CULL_WIREFRAME,
	};

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
		Vec3 position;
		Quaternion rotation;

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
			Trans T(position.x(), position.y(), position.z());
			Scale S(scale.x(), scale.y(), scale.z());
			Mat4 R = rotation.ToMatrix();

			return S * R * T;
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

		inline Vec3 Forward() { return rotation.Forward(); }
		inline Vec3 Right() { return rotation.Right(); }
		inline Vec3 Up() { return rotation.Up(); }
	};

	struct SpriteRendererComponent
	{
		Vec4 Color {1.0f, 1.0f, 1.0f, 1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const Vec4& color) : Color(color) {}
	};

	struct TextureComponent
	{
		TextureObject::Name textureID;
	};

	struct CameraComponent
	{
		Camera camera;
		bool isPriority = false;
	};

	struct LightComponent
	{
		//Current Light is just some data
		Vec3 LightColor;
		Vec3 LightPos;
	};

	struct MeshComponent
	{
		Mesh::Name meshID;
	};

	struct MaterialComponent
	{
		Material::Name materialID;
		ShaderObject::Name shaderID;
		RasterizerStateID rasterizerID;

		Material::Name  lightID;
	};
}

#endif // !TRANSFORM_H
