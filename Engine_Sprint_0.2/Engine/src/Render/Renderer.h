#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>
#include "ShaderObject.h"
#include "ZVector.hpp"
#include "ZEntity.h"
#include "StateRasterizer.h"

namespace Azul
{
	class Scene;
	class Material;
	class ArchetypeWorld;
	struct TransformComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ShaderComponent;

	class Renderer
	{
	public:
		Renderer(Scene* scene);

		void Draw(zecs::ArchetypeWorld& world);
	private:
		void RenderAll();
		void ActiveRasterizer();

		struct DrawData
		{

			DrawData()
			{
				transform = nullptr;
				material = nullptr;
				mesh = nullptr;
			}
			DrawData(TransformComponent* trans, MaterialComponent* mat, MeshComponent* mesh)
				: transform(trans), material(mat), mesh(mesh) {};
			DrawData(const DrawData& other) = default;
			DrawData(DrawData&&) = default;

			DrawData& operator = (const DrawData& other) = default;

			TransformComponent* transform;
			MaterialComponent* material;
			MeshComponent* mesh;
		};

	private:
		RasterizerStateID currentState;

		StateRasterizer mStateRasterizerSolid;
		StateRasterizer mStateRasterizerWireframe;

		Scene* scene;
		std::unordered_map<ShaderObject::Name, ZVector<DrawData>> renderBuckets;
	};
}

#endif // !3DRENDERER_H