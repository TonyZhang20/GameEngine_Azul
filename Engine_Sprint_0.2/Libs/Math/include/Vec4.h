//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_Vect4D_H
#define ENGINE_MATH_Vect4D_H

// Includes to handle SIMD register types
#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Math_DLLInterface.h"

#include "Constants.h"
#include "Vec4Proxy.h"
#include "Enum.h"
// This file assumes Framework.h is included in executable for Align16

namespace Azul
{
	// forward declare
	class Mat4;
	class Mat3;
	class Vec3;
	class Scale;
	class Trans;
	class Rot;

	// -----------------------------------------------------------
	// 
	// Vec4 
	//   True 4 element vector... 
	//     its stored in a SIMD friendly struct for cache reasons
	//                         
	//   v4  = | x  y  z  w | 
	//     
	// -----------------------------------------------------------

	class Vec4 final : public Align16
	{
	public:

		// Do your magic here

		// Big 4
		MATH_LIBRARY_API Vec4();
		MATH_LIBRARY_API Vec4(const Vec4&);
		MATH_LIBRARY_API Vec4& operator=(const Vec4&);
		MATH_LIBRARY_API ~Vec4();

		// Big 6
		MATH_LIBRARY_API Vec4(Vec4&& v) = default;
		MATH_LIBRARY_API Vec4& operator=(Vec4&& v) = default;

		// Constructors
		MATH_LIBRARY_API Vec4(const float in_x, const float in_y, const float in_z, const float in_w);

		// Forces User to explicitly do the cast or constructor call with explicit
		MATH_LIBRARY_API Vec4(const Vec3& v, const float w);

		// Bracket
		MATH_LIBRARY_API float& operator[] (const enum x_enum);
		MATH_LIBRARY_API float& operator[] (const enum y_enum);
		MATH_LIBRARY_API float& operator[] (const enum z_enum);
		MATH_LIBRARY_API float& operator[] (const enum w_enum);

		MATH_LIBRARY_API float operator[] (const enum x_enum) const;
		MATH_LIBRARY_API float operator[] (const enum y_enum) const;
		MATH_LIBRARY_API float operator[] (const enum z_enum) const;
		MATH_LIBRARY_API float operator[] (const enum w_enum) const;

		// Accessors - Robin's suggestion
		MATH_LIBRARY_API void x(const float v);
		MATH_LIBRARY_API void y(const float v);
		MATH_LIBRARY_API void z(const float v);
		MATH_LIBRARY_API void w(const float v);

		MATH_LIBRARY_API float x()const;
		MATH_LIBRARY_API float y()const;
		MATH_LIBRARY_API float z()const;
		MATH_LIBRARY_API float w()const;

		// add operators
		MATH_LIBRARY_API Vec4 operator + (void) const;
		MATH_LIBRARY_API Vec4 operator + (const Vec4& inV) const;
		MATH_LIBRARY_API Vec4& operator += (const Vec4& inV);

		// sub operators
		MATH_LIBRARY_API Vec4 operator - (const Vec4& inV) const;
		MATH_LIBRARY_API Vec4& operator -= (const Vec4& inV);
		MATH_LIBRARY_API Vec4 operator - (void) const;

		// scale operators
		MATH_LIBRARY_API Vec4 operator * (const float scale) const;
		MATH_LIBRARY_API friend Vec4 operator *(const float scale, const Vec4& inV);
		MATH_LIBRARY_API Vec4& operator *= (const float scale);
		MATH_LIBRARY_API Vec3 getVec3() const;

		// Vec4 * Mat4
		MATH_LIBRARY_API Vec4 operator * (const Mat4& m) const;
		MATH_LIBRARY_API Vec4& operator *= (const Mat4& m);

		// Vec4 functions
		MATH_LIBRARY_API Vec4& norm(void);
		MATH_LIBRARY_API Vec4 getNorm(void) const;
		MATH_LIBRARY_API float dot(const Vec4& vIn) const;
		MATH_LIBRARY_API Vec4Proxy len() const;

		// set
		MATH_LIBRARY_API void set(const float inX, const float inY, const float inZ, const float inW);
		MATH_LIBRARY_API void set(const Vec4& A);
		MATH_LIBRARY_API void set(const Vec3& v, const float w);

		// comparison
		MATH_LIBRARY_API bool isEqual(const Vec4& v, const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isZero(const float epsilon = MATH_TOLERANCE) const;

		// for printing
		MATH_LIBRARY_API void Print(const char* pName) const;

	private:

		friend Mat4;
		friend Scale;
		friend Trans;
		friend Rot;
		friend Mat3;
		friend Vec3;

		union
		{
			/*                          */
			/*   v4  = | x  y  z  w |   */
			/*                          */

			__m128	_mv;

			// anonymous struct
			struct
			{
				float _vx;
				float _vy;
				float _vz;
				float _vw;
			};
		};
	};

}

#endif

//--- End of File ---
