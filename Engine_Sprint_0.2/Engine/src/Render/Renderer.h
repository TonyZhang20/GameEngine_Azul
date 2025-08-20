#ifndef RENDERER_H
#define RENDERER_H

#include <unordered_map>
#include "Layer.h"
#include "Components.h"
#include "ShaderObject.h"
#include "ZVector.hpp"
#include "ZEntity.h"

#include "StateHeaders.h"

namespace Azul
{
	class Scene;
	class Material;
	class ArchetypeWorld;
	struct TransformComponent;
	struct MaterialComponent;
	struct MeshComponent;
	struct ShaderComponent;

	/// <summary>
	/// Renderer Layer
	/// </summary>
	class Renderer final : public Layer
	{
	public:
		Renderer();
		Renderer(const char* name);

		Renderer(int order);
		Renderer(int order, const char* name);
		

		void Draw(ZVector<RenderPacket>& packets, ZEntity& cam);

		void InitRenderer();
		bool OnWindowResize(WindowResizeEvent&);
	private:
		void RenderAll(ZEntity& cam);
		void ActiveRasterizer();
		void SetDefaultTargetMode();
		void ClearDepthStencilBuffer();

		virtual void Awake() override;
		virtual void Start() override;
		virtual void OnRender(float deltaTime) override;
		virtual void OnEvent(class Event& event) override;
		
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
		RasterizerStateID currentState = RasterizerStateID::NOTINITIALIZE;

		StateRasterizer mStateRasterizerSolid;
		StateRasterizer mStateRasterizerWireframe;

		StateBlend mBlendStateOff;
		StateBlend mBlendStateAlpha;

		StateViewport mViewport;

		StateRenderTargetView mStateRenderTargetView; //Omset
		StateDepthStencil mStateDepthStencil;		  //深度模板状态
		StateDepthStencilView mDepthStencilView;	  //深度模板窗口
		StateDepthStencilBuffer mDepthStencilBuffer;  //深度模板

		std::unordered_map<ShaderObject::Name, ZVector<DrawData>> renderBuckets;

		unsigned int height;
		unsigned int width;
	};
}

#endif // !3DRENDERER_H