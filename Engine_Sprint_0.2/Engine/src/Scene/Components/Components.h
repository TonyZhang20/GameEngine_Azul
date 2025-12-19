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
#include "ScriptableEntity.h"

#define ErrorID 0xfffff
#define ErrorVersion 0xff
namespace Azul
{
	enum class RasterizerStateID
	{
		NOTINITIALIZE = 0,
		D3D11_FILL_SOLID,
		D3D11_CULL_WIREFRAME,
	};

	struct TagComponent
	{
		char tag[256];

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
		Quat rotation;
		Vec3 deltaRot;

		zecs::EntityID parent;

		TransformComponent()
		{
			parent.index = ErrorID;
			parent.version = ErrorVersion;
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

			Quat qx = Quat::FromAxisAngle(Vec3(1, 0, 0), deltaRot[x] * MATH_PI_180);
			Quat qy = Quat::FromAxisAngle(Vec3(0, 1, 0), deltaRot[y] * MATH_PI_180);
			Quat qz = Quat::FromAxisAngle(Vec3(0, 0, 1), deltaRot[z] * MATH_PI_180);

			Quat final = qx * qy * qz * rotation;

			Mat4  R(final);

			return S * R * T;
		};

		inline Mat4 GetWorldMatrix() const
		{
			if (parent.index == ErrorID)
			{
				return GetLocalMatrix();
			}
			else
			{
				//Parent.GetComponent<Transform>().GetWorldMatrix() * GetLocalMatrix();
				//Trans TransA(position.x(), position.y(), position.z());
				//Scale Scale(scale.x(), scale.y(), scale.z());
				//p * s * r * t
				return GetLocalMatrix();
			}
		};

		inline Vec3 Forward() { return rotation.Forward(); }
		inline Vec3 Right() { return rotation.Right(); }
		inline Vec3 Up() { return rotation.Up(); }

		inline void SetForward(const Vec3& forward, const Vec3* up = nullptr) { rotation.SetForward(forward, up); }

		inline void SetScale(float s) { this->scale = Vec3(s, s, s); };
		inline void SetPosition(const Vec3& pos) { this->position.set(pos); };

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
		CameraComponent() = default;
		CameraComponent(const CameraComponent& other) : camera(other.camera), isPriority(other.isPriority) {}

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
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;

		Mesh::Name meshID;
	};

	struct MaterialComponent
	{
		MaterialComponent() = default;
		MaterialComponent(const MaterialComponent&) = default;

		Material::Name materialID;
		ShaderObject::Name shaderID;
		RasterizerStateID rasterizerID;

		//TODO: Light ID
		Material::Name  lightID;
	};

	struct RenderPacket
	{
		TransformComponent* transform;   // 世界矩阵
		MeshComponent* mesh;             // 顶点/索引数据
		MaterialComponent* material;     // Shader、纹理、颜色等
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		//function pointer
		ScriptableEntity*(*InstantiateScript)();

		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc)
				{	delete nsc->Instance;
					nsc->Instance = nullptr; };
		}
	};
}

#endif // !TRANSFORM_H
