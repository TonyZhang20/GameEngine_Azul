//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_LIGHT_TEXTURE_H
#define GRAPHICS_OBJECT_LIGHT_TEXTURE_H

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{

	class GraphicsObject_LightTexture :public GraphicsObject
	{
	public:
		GraphicsObject_LightTexture(Material* mat, Mesh* mesh, Vec3& lightColor, Vec3& lightPos);

		GraphicsObject_LightTexture() = delete;
		GraphicsObject_LightTexture(const GraphicsObject_LightTexture &) = delete;
		GraphicsObject_LightTexture &operator = (const GraphicsObject_LightTexture &) = delete;
		virtual ~GraphicsObject_LightTexture();

		virtual void SetGpu(Camera* pCam);
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here

		Vec3 *poLightColor;
		Vec3 *poLightPos;
	};

}

#endif

// --- End of File ---
