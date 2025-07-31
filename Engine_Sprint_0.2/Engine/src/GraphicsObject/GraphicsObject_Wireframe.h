//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_WIREFRAME_H
#define GRAPHICS_OBJECT_WIREFRAME_H

#include "GraphicsObject.h"
namespace Azul
{
	class GraphicsObject_Wireframe :public GraphicsObject
	{
	public:
		GraphicsObject_Wireframe() = delete;
		GraphicsObject_Wireframe(Material* mat, Mesh* model);

		GraphicsObject_Wireframe(const GraphicsObject_Wireframe&) = delete;
		GraphicsObject_Wireframe& operator = (const GraphicsObject_Wireframe&) = delete;
		virtual ~GraphicsObject_Wireframe() = default;

		virtual void Draw() override;
		virtual void RestoreState() override;
		virtual void SetState() override;
		// data:  place uniform instancing here

	};
}


#endif

// ---  End of File ---
