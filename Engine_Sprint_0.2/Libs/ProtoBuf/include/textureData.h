//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#ifndef TEXTURE_DATA_H
#define TEXTURE_DATA_H

#include "ProtoBufEngine_DLLInterface.h"

#include "ProtoVersion.h"
#include "textureData.pb.h"

class textureData
{
public:
	static const unsigned int FILE_NAME_SIZE = 64;

public:

	enum class TEXTURE_EFORMAT
	{
		EFORMAT_BGR = 0,
		EFORMAT_RGB = 1,
		EFORMAT_RGBA = 2,
		EFORMAT_BGRA = 3,
		DEFAULT = EFORMAT_RGBA
	};

	enum class TEXTURE_NCOMPONENT
	{
		NCOMPONENT_BGR = 0,
		NCOMPONENT_RGB = 1,
		NCOMPONENT_RGBA = 2,
		NCOMPONENT_BGRA = 3,
		DEFAULT = NCOMPONENT_RGBA
	};

	enum class TEXTURE_TYPE
	{
		JPEG = 0,
		PNG = 1,
		BMP = 2,
		GIF = 3,
		TGA = 4,
		DEFAULT = PNG
	};

	enum class TEXTURE_MAG_FILTER
	{
		MAG_NEAREST = 0,
		MAG_LINEAR = 1,
		DEFAULT = MAG_LINEAR
	};

	enum class TEXTURE_MIN_FILTER
	{
		MIN_NEAREST = 0,
		MIN_LINEAR = 1,
		MIN_NEAREST_MIPMAP_NEAREST = 2,
		MIN_LINEAR_MIPMAP_NEAREST = 3,
		MIN_NEAREST_MIPMAP_LINEAR = 4,
		MIN_LINEAR_MIPMAP_LINEAR = 5,
		DEFAULT = MIN_LINEAR
	};

	enum class TEXTURE_WRAP
	{
		WRAP_CLAMP_TO_EDGE = 0,
		WRAP_MIRRORED_REPEAT = 1,
		WRAP_REPEAT = 2,
		DEFAULT = WRAP_CLAMP_TO_EDGE
	};

	enum class TEXTURE_COMPONENT
	{
		BYTE = 0,
		UNSIGNED_BYTE = 1,
		SHORT = 2,
		UNSIGNED_SHORT = 3,
		INT = 4,
		UNSIGNED_INT = 5,
		FLOAT = 6,
		DOUBLE = 7,
		DEFAULT = UNSIGNED_BYTE
	};

public:
	PROTOBUF_ENGINE_LIBRARY_API textureData();
	PROTOBUF_ENGINE_LIBRARY_API textureData(const textureData &);
	PROTOBUF_ENGINE_LIBRARY_API textureData &operator = (const textureData &);
	PROTOBUF_ENGINE_LIBRARY_API ~textureData();

	PROTOBUF_ENGINE_LIBRARY_API textureData(bool                enabled,
											TEXTURE_TYPE        textType,
											TEXTURE_MAG_FILTER  magFilter,
											TEXTURE_MIN_FILTER  minFilter,
											TEXTURE_WRAP        wrapS,
											TEXTURE_WRAP        wrapT,
											TEXTURE_EFORMAT     eFormat,
											TEXTURE_NCOMPONENT  nComponent,
											unsigned int        width,
											unsigned int        height,
											unsigned int        component,
											unsigned int        bits,
											bool                as_is,
											TEXTURE_COMPONENT   pixel_type,
											const char *pFileName,
											unsigned int   dataSize,
											unsigned char *poData);

	// Serialization 
	PROTOBUF_ENGINE_LIBRARY_API void Serialize(textureData_proto &out) const;
	PROTOBUF_ENGINE_LIBRARY_API void Deserialize(const textureData_proto &in);

	PROTOBUF_ENGINE_LIBRARY_API void Print(const char *const pName) const;

public:
	char                pVersion[protoVersion::VERSION_NUM_BYTES];

	bool                enabled;
	TEXTURE_TYPE        textType;
	TEXTURE_MAG_FILTER  magFilter;
	TEXTURE_MIN_FILTER  minFilter;
	TEXTURE_WRAP        wrapS;
	TEXTURE_WRAP        wrapT;

	TEXTURE_EFORMAT     eFormat;
	TEXTURE_NCOMPONENT  nComponent;

	unsigned int        width;
	unsigned int        height;

	//     N=#comp     components
	//       1           grey
	//       2           grey, alpha
	//       3           red, green, blue
	//       4           red, green, blue, alpha
	unsigned int        component;  // bits per channel
	unsigned int        bits;       // bits per channel 8, 16, 32 per color

	bool                as_is;
	TEXTURE_COMPONENT   pixel_type;

	char                pFileName[FILE_NAME_SIZE];
	unsigned int        dataSize;
	unsigned char *poData;

};

#endif

// --- End of File ---
