#include "Material.h"
#include "CameraNodeManager.h"

namespace Azul
{
	Material::Material()
		: pShader(nullptr), pTexture(nullptr), pWorldMatrix(nullptr)
	{
	}

	Material::~Material()
	{
		// 不负责释放 Shader/Texture，这些由资源管理器控制
	}

	void Material::SetShader(ShaderObject* pShaderObj)
	{
		this->pShader = pShaderObj;
	}

	void Material::SetTexture(TextureObject* pTexObj)
	{
		this->pTexture = pTexObj;
	}

	void Material::SetWorldMatrix(Mat4* pWorld)
	{
		this->pWorldMatrix = pWorld;
	}


	void Material::Bind()
	{
		assert(pShader);
		assert(pWorldMatrix);

		pShader->ActivateShader();
		pShader->TransferWorldViewProj(CameraNodeManager::GetMainCam(), pWorldMatrix);
		pShader->ActivateCBV();

		if (pTexture)
		{
			pTexture->ActivateTexture(); // 激活 SRV 和 Sampler
		}
	}

	void Material::TransferWorldViewProj(Mat4* pView, Mat4* pProj)
	{
		this->pViewMatrix = pView;
		this->pProjMatrix = pProj;
	}

}