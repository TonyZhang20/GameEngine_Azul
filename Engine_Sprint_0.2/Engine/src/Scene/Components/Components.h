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

			Rot R(final);

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
		inline void LookAt(const Vec3& target, const Vec3& worldUp = Vec3(0, 1, 0))
		{
			Vec3 fwd = target - this->position;
			if (fwd.len() < 0.00001f)
				return; // target 太近，方向不确定
			fwd.norm();

			Vec3 up = worldUp;
			if (up.len() < 0.00001f)
				up = Vec3(0.0f, 1.0f, 0.0f);
			up.norm();

			// 若 fwd 和 up 近平行，换一个 up（避免 cross=0）
			if (fabsf(fwd.dot(up)) > 0.9990f)
			{
				// 选一个不平行的备用 up
				up = (fabsf(fwd.y()) < 0.9990f) ? Vec3(0.0f, 1.0f, 0.0f) : Vec3(1.0f, 0.0f, 0.0f);
			}

			// 右手系/左手系这里你当前是 Left-handed(Forward=+Z)，
			Vec3 right = up.cross(fwd);
			if (right.len() < 0.00001f)
				return;

			right.norm();

			Vec3 realUp = fwd.cross(right);
			realUp.norm();

			Rot R;
			// 你在 MouseInput 里就是这样写 Rot 的：right 在 m0/m1/m2，up 在 m4/m5/m6，fwd 在 m8/m9/m10
			R[m0] = right.x();  R[m1] = right.y();  R[m2] = right.z();
			R[m4] = realUp.x(); R[m5] = realUp.y(); R[m6] = realUp.z();
			R[m8] = fwd.x();    R[m9] = fwd.y();    R[m10] = fwd.z();

			Quat q(R);
			q.norm();
			this->rotation = q;
		}

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
