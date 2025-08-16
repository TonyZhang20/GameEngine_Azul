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
#include "MathExportAPI.h"
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
		MATHLIBRARY_API Vec3();
		MATHLIBRARY_API Vec3(const Vec3& inV);
		MATHLIBRARY_API Vec3& operator=(const Vec3& v);
		MATHLIBRARY_API ~Vec3();

		// Big 6
		MATHLIBRARY_API Vec3(Vec3&&) = default;
		MATHLIBRARY_API Vec3& operator = (Vec3&&) = default;

		// Constructors
		MATHLIBRARY_API Vec3(const float in_x, const float in_y, const float in_z);

		// Forces User to explicitly do the cast or constructor call with explicit
		MATHLIBRARY_API explicit Vec3(const Vec4& v);
		MATHLIBRARY_API Vec3& operator=(const Vec4& v);

		// Bracket
		MATHLIBRARY_API float& operator[] (const enum x_enum);
		MATHLIBRARY_API float& operator[] (const enum y_enum);
		MATHLIBRARY_API float& operator[] (const enum z_enum);

		MATHLIBRARY_API float operator[] (const enum x_enum) const;
		MATHLIBRARY_API float operator[] (const enum y_enum) const;
		MATHLIBRARY_API float operator[] (const enum z_enum) const;

		// Accessors - Robin's suggestion
		MATHLIBRARY_API void x(const float v);
		MATHLIBRARY_API void y(const float v);
		MATHLIBRARY_API void z(const float v);

		MATHLIBRARY_API float x() const;
		MATHLIBRARY_API float y() const;
		MATHLIBRARY_API float z() const;

		// add operators
		MATHLIBRARY_API Vec3 operator + (void) const;
		MATHLIBRARY_API Vec3 operator + (const Vec3& inV) const;
		MATHLIBRARY_API Vec3& operator += (const Vec3& inV);

		// sub operators
		MATHLIBRARY_API Vec3 operator - (const Vec3& inV) const;
		MATHLIBRARY_API Vec3& operator -= (const Vec3& inV);
		MATHLIBRARY_API Vec3 operator - (void) const;

		// scale operators
		MATHLIBRARY_API Vec3 operator * (const float scale) const;
		MATHLIBRARY_API friend Vec3 operator *(const float scale, const Vec3& inV);
		MATHLIBRARY_API Vec3& operator *= (const float scale);

		// Vec3 * Mat3
		MATHLIBRARY_API Vec3 operator* (const Mat3& m) const;
		MATHLIBRARY_API Vec3& operator*= (const Mat3& m);

		MATHLIBRARY_API float Distance(const Vec3& vIn);

		// Vector functions
		MATHLIBRARY_API Vec3& norm(void);
		MATHLIBRARY_API Vec3 getNorm(void) const;
		MATHLIBRARY_API float dot(const Vec3& vIn) const;
		MATHLIBRARY_API Vec3 cross(const Vec3& vIn) const;
		MATHLIBRARY_API Vec3Proxy len(void) const;
		MATHLIBRARY_API float getAngle(const Vec3& vIn) const;

		// set
		MATHLIBRARY_API void set(const float inX, const float inY, const float inZ);
		MATHLIBRARY_API void set(const Vec3& A);
		MATHLIBRARY_API void set(const Vec4& A);

		// comparison
		MATHLIBRARY_API bool isEqual(const Vec3& v, const float epsilon = MATH_TOLERANCE) const;
		MATHLIBRARY_API bool isZero(const float epsilon = MATH_TOLERANCE) const;

		// for printing
		MATHLIBRARY_API void Print(const char* pName) const;

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
