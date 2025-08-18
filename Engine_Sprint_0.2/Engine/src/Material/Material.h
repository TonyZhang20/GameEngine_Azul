#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderObject.h"
#include "TextureObject.h"

namespace Azul
{
	class Material : public DLink
	{
	public:

		enum Name
		{
			None,
			DefaultMaterial,
			LightTextureA,
			FlatTextureA,
			WireFramNoTextureA,
		};

		Material();
		Material(ShaderObject* shdaer, TextureObject* texture);
		Material(const Material&) = delete;
		Material& operator = (const Material&);
		~Material();

		void Init(ShaderObject* shader, TextureObject* tex, Name _name);

		void SetShader(ShaderObject* pShaderObj);
		void SetTexture(TextureObject* pTexObj);
		void SetName(Name _name);

		void BindShdaer();
		void BindTexture();
		void TransferWorldViewProj(Camera* pCam, Mat4* pWorld);

		inline ShaderObject* GetShader() { return pShader; }
		inline TextureObject* GetTexture() { return pTexture; }

		virtual void Wash() override;
		virtual bool Compare(DLink* targetNode) override;

	private:
		ShaderObject* pShader;
		TextureObject* pTexture;
		Name name;
	};
}


#endif // !MATERIAL_H
