#include "Material.h"
#include "Camera.h"
#include "MaterialMan.h"

namespace Azul
{
	Material::Material()
		: pShader(nullptr), pTexture(nullptr)
	{
		this->name = None;
	}

	Material::Material(ShaderObject* shdaer, TextureObject* texture)
		: pShader(shdaer), pTexture(texture)
	{

	}

	Material& Material::operator=(const Material& inMat)
	{
		// TODO: insert return statement here
		this->pShader = inMat.pShader;
		this->pTexture = inMat.pTexture;
		this->name = inMat.name;

		return *this;
	}

	Material::~Material()
	{

	}

	void Material::Init(ShaderObject* shader, TextureObject* tex, Name _name)
	{
		SetShader(shader);
		SetTexture(tex);
		SetName(_name);
	}

	void Material::SetShader(ShaderObject* pShaderObj)
	{
		this->pShader = pShaderObj;
	}

	void Material::SetTexture(TextureObject* pTexObj)
	{
		this->pTexture = pTexObj;
	}

	void Material::SetName(Name _name)
	{
		this->name = _name;
	}

	void Material::BindShdaer()
	{
		assert(pShader, "Shader is Required");

		pShader->ActivateShader();
		pShader->ActivateCBV();
	}

	void Material::BindTexture()
	{
		if (pTexture)
		{
			pTexture->ActivateTexture();
		}
	}

	void Material::TransferWorldViewProj(Camera* pCam, Mat4* pWorld)
	{
		assert(pShader);
		pShader->TransferWorldViewProj(pCam, pWorld);
	}

	void Material::Wash()
	{
		this->pShader = nullptr;
		this->pTexture = nullptr;
		this->name = None;
	}
	bool Material::Compare(DLink* targetNode)
	{
		Material* target = (Material*)targetNode;

		return this->name == target->name;
	}
}