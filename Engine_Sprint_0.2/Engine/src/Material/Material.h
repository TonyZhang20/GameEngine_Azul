#ifndef MATERIAL_H
#define MATERIAL_H

#include "ShaderObject.h"
#include "TextureObject.h"
#include "MathEngine.h"

namespace Azul
{
	class Material
	{
	public:
		Material();
		Material(const Material&) = delete;
		Material& operator = (const Material&) = delete;
		~Material();

		void SetShader(ShaderObject* pShaderObj);
		void SetTexture(TextureObject* pTexObj); // 可为空（如 ColorByVertex）
		void SetWorldMatrix(Mat4* pWorld);

		// Render时绑定资源（仅激活Shader、CBV、贴图）
		void Bind();

		// 让外部显式调用传入ViewProj矩阵
		void TransferWorldViewProj(Mat4* pView, Mat4* pProj);

	private:
		ShaderObject* pShader;
		TextureObject* pTexture;
		Mat4* pWorldMatrix;
	};
}


#endif // !MATERIAL_H
