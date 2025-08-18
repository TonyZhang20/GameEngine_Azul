
#include "Scene.h"
#include "ArchetypeWorld.h"
#include "Components.h"
#include "Renderer.h"
#include "MaterialMan.h"
#include "Material.h"
#include "ShaderObjectNodeManager.h"
#include "MaterialMan.h"
#include "MeshNodeManager.h"
namespace Azul
{
	Renderer::Renderer(Scene* scene) : scene(scene), mStateRasterizerSolid(), mStateRasterizerWireframe()
	{
		this->mStateRasterizerSolid.Initialize(D3D11_FILL_SOLID, D3D11_CULL_FRONT);
		this->mStateRasterizerWireframe.Initialize(D3D11_FILL_WIREFRAME, D3D11_CULL_FRONT);
	}

	void Renderer::Draw(zecs::ArchetypeWorld& world)
	{
		auto entities = world.Query<TransformComponent, MeshComponent, MaterialComponent>();

		for (auto entity : entities)
		{
			TransformComponent& transform = world.GetComponent<TransformComponent>(entity);
			MeshComponent& mesh = world.GetComponent<MeshComponent>(entity);
			MaterialComponent& material = world.GetComponent<MaterialComponent>(entity);

			if (renderBuckets.find(material.shaderID) == renderBuckets.end())
			{
				renderBuckets[material.shaderID] = ZVector<DrawData>();
			}

			renderBuckets[material.shaderID].emplace_back(DrawData{ &transform, &material, &mesh });
		}

		RenderAll();
	}

	void Renderer::RenderAll()
	{
		using Transform = TransformComponent;

		auto& zCam = this->scene->GetMainCamera();

		ZEntity camEntity{ zCam, this->scene };

		auto& camTrans = camEntity.GetComponent<Transform>();
		auto& camComp = camEntity.GetComponent<CameraComponent>();

		for (auto& bucket : renderBuckets)
		{
			ShaderObjectNode* pShader = ShaderObjectNodeManager::Find(bucket.first);

			pShader->ActivateShader();
			pShader->ActivateCBV();

			pShader->TransferProj(camComp.camera.getProjMatrix());
			pShader->TransferView(camComp.camera.getViewMatrix(camTrans.rotation, camTrans.position));

			for (auto& components : bucket.second)
			{
				Material* mat = MaterialMan::Find(components.material->materialID);
				MeshNode* mesh = MeshNodeManager::Find(components.mesh->meshID);
				Transform* trans = components.transform;

				//Set State
				if (currentState != components.material->rasterizerID)
				{
					currentState = components.material->rasterizerID;
					ActiveRasterizer();
				}

				//world matrix
				Mat4 world = trans->GetWorldMatrix();
				pShader->TransferWorld(world);

				//material bind texture
				mat->BindTexture();

				/*
				方案1： 全部Light都传，我管你这啊哪的
				方案2： ShaderObject 处理Light，把所有光传给他，他自己挑
				目前使用方案2
				*/
				if (components.material->lightID != Material::None)
				{
					//auto& light = LightManager::Find(lightID)
					//pShader->TransferLight(light.position, light.color);
				}
				
				//Draw
				mesh->ActivateMesh();
				mesh->RenderIndexBuffer();
			}
		}
	}

	void Renderer::ActiveRasterizer()
	{
		switch (currentState)
		{
		case RasterizerStateID::D3D11_FILL_SOLID:
		{
			this->mStateRasterizerSolid.Activate();
			break;
		}
		case RasterizerStateID::D3D11_CULL_WIREFRAME:
			this->mStateRasterizerWireframe.Activate();
			break;
		default:
			break;
		}
	}
}

