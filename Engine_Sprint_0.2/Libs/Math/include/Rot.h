//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_ROT_4x4_H
#define ENGINE_MATH_ROT_4x4_H

#include "Math_DLLInterface.h"

#include "Mat4.h"

namespace Azul
{
	class Quat;

	class Rot final : public Mat4
	{
	public:

		// Do your magic here

		//Big 4
		MATH_LIBRARY_API Rot();
		MATH_LIBRARY_API Rot& operator = (const Rot&) = default;
		MATH_LIBRARY_API Rot(const Rot&) = default;
		MATH_LIBRARY_API ~Rot() = default;

		//Big 6
		MATH_LIBRARY_API Rot(Rot&&) = default;
		MATH_LIBRARY_API Rot& operator = (Rot&&) = default;

		MATH_LIBRARY_API explicit Rot(const Quat &);
		MATH_LIBRARY_API Rot & operator = (const Quat &);

		MATH_LIBRARY_API Rot(const Mat4&);
		MATH_LIBRARY_API explicit Rot(const enum Identity_enum);
		MATH_LIBRARY_API Rot(const Rot1 type, const float angle);
		MATH_LIBRARY_API Rot(const Rot3 mode, const float angle_x, const float angle_y, const float angle_z);
		MATH_LIBRARY_API Rot(const Axis mode, const Vec3& vAxis, const float angle_radians);
		MATH_LIBRARY_API Rot(const Orient, const Vec3& dof, const Vec3& up);

		MATH_LIBRARY_API void set(const Quat& q);
		MATH_LIBRARY_API void set(const enum Identity_enum);
		MATH_LIBRARY_API void set(const Rot&);
		MATH_LIBRARY_API void set(const Rot1 type, const float angle);
		MATH_LIBRARY_API void set(const Rot3 mode, const float angle_x, const float angle_y, const float angle_z);
		MATH_LIBRARY_API void set(const Axis mode, const Vec3& vAxis, const float angle_radians);
		MATH_LIBRARY_API void set(const Orient, const Vec3& dof, const Vec3& up);

		//Multiply wih hints
		MATH_LIBRARY_API Rot operator * (const Quat &A) const;
		MATH_LIBRARY_API Rot &operator *= (const Quat &A);

		MATH_LIBRARY_API Mat4 operator * (const Mat4& A) const;
		MATH_LIBRARY_API Rot& operator *= (const Mat4& A) = delete;
		//
		MATH_LIBRARY_API Mat4 operator * (const Scale& A) const;
		MATH_LIBRARY_API Rot& operator *= (const Scale& A) = delete;
		//
		MATH_LIBRARY_API Rot operator * (const Rot& A) const;
		MATH_LIBRARY_API Rot& operator *= (const Rot& A);
		//
		MATH_LIBRARY_API Mat4 operator * (const Trans& A) const;
		MATH_LIBRARY_API Rot& operator *= (const Trans& A) = delete;

	};
}

#endif

//--- End of File ---
