//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_VECT3_H
#define ENGINE_MATH_VECT3_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Constants.h"
#include "Vec3Proxy.h"

#include "Math_DLLInterface.h"
// This file assumes Framework.h is included in executable for Align16

namespace Azul
{
	// forward declare
	class Mat4;
	class Mat3;
	class Vec4;
	class Vec2;
	class Trans;
	class Scale;
	class Rot;
	class Quat;
	// -----------------------------------------------------------
	// 
	// Vec3 
	//   True 3 element vector... 
	//     its stored in a SIMD friendly struct for cache reasons
	//     the "w" component is ignored and not set
	//                         
	//   v3  = | x  y  z  - | 
	//     
	// -----------------------------------------------------------

	class Vec3 final : public Align16
	{
	public:
		// Big 4
		MATH_LIBRARY_API Vec3();
		MATH_LIBRARY_API Vec3(const Vec3& inV);
		MATH_LIBRARY_API Vec3& operator=(const Vec3& v);
		MATH_LIBRARY_API ~Vec3();

		// Big 6
		MATH_LIBRARY_API Vec3(Vec3&&) = default;
		MATH_LIBRARY_API Vec3& operator = (Vec3&&) = default;

		// Constructors
		MATH_LIBRARY_API Vec3(const float in_x, const float in_y, const float in_z);

		// Forces User to explicitly do the cast or constructor call with explicit
		MATH_LIBRARY_API explicit Vec3(const Vec4& v);
		MATH_LIBRARY_API Vec3& operator=(const Vec4& v);

		// Bracket
		MATH_LIBRARY_API float& operator[] (const enum x_enum);
		MATH_LIBRARY_API float& operator[] (const enum y_enum);
		MATH_LIBRARY_API float& operator[] (const enum z_enum);

		MATH_LIBRARY_API float operator[] (const enum x_enum) const;
		MATH_LIBRARY_API float operator[] (const enum y_enum) const;
		MATH_LIBRARY_API float operator[] (const enum z_enum) const;

		// Accessors - Robin's suggestion
		MATH_LIBRARY_API void x(const float v);
		MATH_LIBRARY_API void y(const float v);
		MATH_LIBRARY_API void z(const float v);

		MATH_LIBRARY_API float x() const;
		MATH_LIBRARY_API float y() const;
		MATH_LIBRARY_API float z() const;

		// add operators
		MATH_LIBRARY_API Vec3 operator + (void) const;
		MATH_LIBRARY_API Vec3 operator + (const Vec3& inV) const;
		MATH_LIBRARY_API Vec3& operator += (const Vec3& inV);

		// sub operators
		MATH_LIBRARY_API Vec3 operator - (const Vec3& inV) const;
		MATH_LIBRARY_API Vec3& operator -= (const Vec3& inV);
		MATH_LIBRARY_API Vec3 operator - (void) const;

		// scale operators
		MATH_LIBRARY_API Vec3 operator * (const float scale) const;
		MATH_LIBRARY_API friend Vec3 operator *(const float scale, const Vec3& inV);
		MATH_LIBRARY_API Vec3& operator *= (const float scale);

		// Vec3 * Mat3
		MATH_LIBRARY_API Vec3 operator* (const Mat3& m) const;
		MATH_LIBRARY_API Vec3& operator*= (const Mat3& m);

		// Vec3 * Quat
		MATH_LIBRARY_API Vec3 operator* (const Quat &q) const;
		MATH_LIBRARY_API Vec3 &operator*= (const Quat &q);

		MATH_LIBRARY_API float Distance(const Vec3& vIn);

		// Vector functions
		MATH_LIBRARY_API Vec3& norm(void);
		MATH_LIBRARY_API Vec3 getNorm(void) const;
		MATH_LIBRARY_API float dot(const Vec3& vIn) const;
		MATH_LIBRARY_API Vec3 cross(const Vec3& vIn) const;
		MATH_LIBRARY_API Vec3Proxy len(void) const;
		MATH_LIBRARY_API float getAngle(const Vec3& vIn) const;

		// set
		MATH_LIBRARY_API void set(const float inX, const float inY, const float inZ);
		MATH_LIBRARY_API void set(const Vec3& A);
		MATH_LIBRARY_API void set(const Vec4& A);

		// comparison
		MATH_LIBRARY_API bool isEqual(const Vec3& v, const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isZero(const float epsilon = MATH_TOLERANCE) const;

		// for printing
		MATH_LIBRARY_API void Print(const char* pName) const;

	private:

		friend Mat4;
		friend Mat3;
		friend Vec4;
		friend Vec2;
		friend Trans;
		friend Scale;
		friend Rot;

		union
		{
			/*                          */
			/*   v3  = | x  y  z  - |   */
			/*                          */

			__m128	_mv;

			// anonymous struct
			struct
			{
				float _vx;
				float _vy;
				float _vz;
			};
		};
	};
}

#endif

//--- End of File ---
