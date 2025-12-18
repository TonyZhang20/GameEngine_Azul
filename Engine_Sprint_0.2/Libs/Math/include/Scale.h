//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_SCALE_4x4_H
#define ENGINE_MATH_SCALE_4x4_H

#include "Math_DLLInterface.h"

#include "Mat4.h"

namespace Azul
{
	class Scale final : public Mat4
	{
	public:

		// Do your magic here

		// Big 4
		MATH_LIBRARY_API Scale();
		MATH_LIBRARY_API Scale &operator = (const Scale &) = default;
		MATH_LIBRARY_API Scale(const Scale &) = default;
		MATH_LIBRARY_API ~Scale() = default;

		// Big 6
		MATH_LIBRARY_API Scale(Scale &&) = default;
		MATH_LIBRARY_API Scale &operator = (Scale &&) = default;

		// Constructors
		MATH_LIBRARY_API Scale(const float sx, const float sy, const float sz);
		MATH_LIBRARY_API explicit Scale(const Vec3 &vScale);
		MATH_LIBRARY_API explicit Scale(const enum Identity_enum);
		
		// Set 
		MATH_LIBRARY_API void set(const float sx, const float sy, const float sz);
		MATH_LIBRARY_API void set(const Vec3 &vScale);
		MATH_LIBRARY_API void set(const enum Identity_enum);

		// Multiply with hints
		MATH_LIBRARY_API Mat4 operator * (const Mat4 &A) const;
		MATH_LIBRARY_API Scale &operator *= (const Mat4 &A) = delete;

		MATH_LIBRARY_API Scale operator * (const Scale &A) const;
		MATH_LIBRARY_API Scale &operator *= (const Scale &A);

		MATH_LIBRARY_API Mat4 operator * (const Rot &A) const;
		MATH_LIBRARY_API Scale &operator *= (const Rot &A) = delete;

		MATH_LIBRARY_API Mat4 operator * (const Trans &A) const;
		MATH_LIBRARY_API Scale &operator *= (const Trans &A) = delete;

		MATH_LIBRARY_API Mat4 operator * (const Quat& q) const;
		MATH_LIBRARY_API Scale& operator *= (const Quat& q) = delete;

	};
}

#endif

//--- End of File ---
