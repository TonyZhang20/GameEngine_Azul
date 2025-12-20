//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef MESH_DATA_H
#define MESH_DATA_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "meshData.pb.h"
#include "vboData.h"
#include "textureData.h"

class meshData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;

public:
	enum class RENDER_MODE
	{
		MODE_POINTS = 0,
		MODE_LINE = 1,
		MODE_LINE_LOOP = 2,
		MODE_LINE_STRIP = 3,
		MODE_TRIANGLES = 4,
		MODE_TRIANGLE_STRIP = 5,
		MODE_TRIANGLE_FAN = 6,
		DEFAULT = MODE_TRIANGLES
	};

public:
	PROTOBUF_ENGINE_LIBRARY_API meshData();
	PROTOBUF_ENGINE_LIBRARY_API meshData(const meshData &) = delete;
	PROTOBUF_ENGINE_LIBRARY_API meshData &operator = (const meshData &) = delete;
	PROTOBUF_ENGINE_LIBRARY_API ~meshData();

	// Serialization 
	PROTOBUF_ENGINE_LIBRARY_API void Serialize(meshData_proto &out) const;
	PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const meshData_proto &in);

	PROTOBUF_ENGINE_LIBRARY_API void Print(const char *const pName) const;

public:
	char             pVersion[protoVersion::VERSION_NUM_BYTES];

	char             pMeshName[FILE_NAME_SIZE];
	RENDER_MODE		 mode;
	unsigned int     triCount;
	unsigned int     vertCount;

	vboData          vbo_vert;
	vboData          vbo_norm;
	vboData          vbo_uv;
	vboData          vbo_index;
	vboData          vbo_color;
};

#endif

// --- End of File ---
