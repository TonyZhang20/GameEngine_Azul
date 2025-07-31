//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_CONST_COLOR_H
#define GRAPHICS_OBJECT_CONST_COLOR_H

#include "GraphicsObject.h"
#include "TextureObject.h"

namespace Azul
{

	class GraphicsObject_ConstColor :public GraphicsObject
	{
	public:
		GraphicsObject_ConstColor(Mesh* mesh, Material* mat, Vec3& LightColor);

		GraphicsObject_ConstColor() = delete;
		GraphicsObject_ConstColor(const GraphicsObject_ConstColor &) = delete;
		GraphicsObject_ConstColor &operator = (const GraphicsObject_ConstColor &) = delete;
		virtual ~GraphicsObject_ConstColor();

		// Rendermaterial contract
		virtual void Draw() override;
		virtual void RestoreState() override;

		// data:  place uniform instancing here
		Vec3 *poLightColor;

	};

}

#endif


// --- End of File ---
