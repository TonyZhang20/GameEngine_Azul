//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef MODEL_H
#define MODEL_H

#include <d3d11.h>
#include "MathEngine.h"
#include "Camera.h"
#include "DLink.h"
#include "BufferCBV_vs.h"

namespace Azul
{
	// Vertex data for a colored cube.
	struct Vec3f
	{
		Vec3f() : x(0), y(0), z(0) {}
		Vec3f(float in_x, float in_y, float in_z)
			: x(in_x), y(in_y), z(in_z)
		{
		}

		float x;
		float y;
		float z;
	};

	struct Vec2f
	{
		Vec2f() : u(0), v(0) {}
		Vec2f(float in_u, float in_v)
			: u(in_u), v(in_v)
		{
		}
		float u;
		float v;
	};

	struct VertexColor
	{
		Vec3f Color;
	};

	struct VertexPos
	{
		Vec3f Position;
	};

	struct VertexNorm
	{
		Vec3f Norm;
	};

	struct VertexTexCoord
	{
		Vec2f TexCoord;
	};

	class Mesh
	{
	public:
		enum class Name
		{
			CUBE = 0,
			PYRAMID,
			A,
			B,
			C,
			D,
			E,
			NULL_MESH,
			NOT_INITIALIZED
		};

	public:
		Mesh();
		Mesh(Mesh& copyModel) = delete;
		Mesh& operator = (Mesh& copyModel) = delete;
		virtual ~Mesh();

		Mesh(unsigned int numVerts, unsigned int numTriangles);

		void Wash();
		bool Compare(DLink* pTarget);
		char* GetName();
		void SetName(Mesh::Name _name);
		void Initialize(unsigned int numVerts, unsigned int numTriangles);

		virtual void ActivateMesh() = 0;
		virtual void RenderIndexBuffer() = 0;

		// ---------------------------------------------
		// 	   Data:
		// ---------------------------------------------
		Name name;

		unsigned int numVerts;
		unsigned int numIndices;
	};

}


#endif

// --- End of File ---
