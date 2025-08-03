//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#include "StringThis.h"

namespace Azul
{
	StringThis::StringThis(Mesh::Name status)
	{
		switch (status)
		{
		case Mesh::Name::CUBE:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::CUBE));
			break;

		case Mesh::Name::NOT_INITIALIZED:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::NOT_INITIALIZED));
			break;

		case Mesh::Name::NULL_MESH:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Mesh::NULL_MESH));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(ShaderObject::Name status)
	{
		switch (status)
		{
		case ShaderObject::Name::NullShader:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::NullShader));
			break;

		case ShaderObject::Name::ColorByVertex:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::ColorByVertex));
			break;

		case ShaderObject::Name::FlatTexture:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::FlatTexture));
			break;

		case ShaderObject::Name::LightTexture:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::LightTexture));
			break;

		case ShaderObject::Name::Uninitialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(ShaderObject::Uninitialized));
			break;

		default:
			assert(false);
		}
	}

	StringThis::StringThis(Camera::Name status)
	{
		switch (status)
		{
		case Azul::Camera::Name::Default:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::Default));
			break;
		case Azul::Camera::Name::NULL_CAMERA:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::NULL_CAMERA));
			break;
		case Azul::Camera::Name::NOT_INITIALIZED:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::NOT_INITIALIZED));
			break;

		case Azul::Camera::Name::MAIN_CAMERA:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::MAIN_CAMERA));
			break;

		case Azul::Camera::Name::CAMERA_A:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_A));
			break;

		case Azul::Camera::Name::CAMERA_B:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_B));
			break;

		case Azul::Camera::Name::CAMERA_C:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_C));
			break;

		case Azul::Camera::Name::CAMERA_D:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(Camera::CAMERA_D));
			break;
		default:
			break;
		}
	}

	StringThis::StringThis(TextureObject::Name status)
	{
		switch (status)
		{
		case TextureObject::Name::NullTexture:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::NullShader));
			break;

		case TextureObject::Name::Duckweed:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::Duckweed));
			break;

		case TextureObject::Name::Uninitialized:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::Uninitialized));
			break;

		case TextureObject::Name::A:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::A));
			break;

		case TextureObject::Name::B:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::B));
			break;

		case TextureObject::Name::C:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::C));
			break;

		case TextureObject::Name::D:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::D));
			break;

		case TextureObject::Name::E:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::E));
			break;

		case TextureObject::Name::CHECKBOARD:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::CHECKBOARD));
			break;
		case TextureObject::Name::SCENE_WINDOW:
			strcpy_s(this->buffer, BUFFER_SIZE, STRING_ME(TextureObject::SCENE_WINDOW));
			break;

		default:
			assert(false);
		}
	}


	StringThis::operator char* ()
	{
		return this->buffer;
	}

}


// --- End of File ---