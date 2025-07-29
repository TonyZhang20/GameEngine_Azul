//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef GRAPHICS_OBJECT_H
#define GRAPHICS_OBJECT_H

#include "MathEngine.h"
#include "ShaderObject.h"
#include "Render.h"
#include "Mesh.h"

namespace Azul
{
	class GraphicsObject : public Render
	{
	public:
		GraphicsObject(Mesh* model, ShaderObject* pShaderObj);
		GraphicsObject(const GraphicsObject&) = delete;
		GraphicsObject& operator = (const GraphicsObject&) = delete;
		virtual ~GraphicsObject() override;

		void Render();
		void SetWorld(Mat4& _world);

		Mesh* GetModel() const;
		Mat4& GetWorld();

	protected:
		GraphicsObject();

	protected:
		Mesh* pMesh; //will delete by manager
		Mat4* poWorld;
		ShaderObject* pShaderObj; //will delete by manager
	};

}

#endif

// --- End of File ---
