//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_MATRIX_3x3_H
#define ENGINE_MATH_MATRIX_3x3_H

#include "Vec4.h"
#include "Mat4.h"
#include "MathExportAPI.h"
namespace Azul
{
	class Mat3 final : public Align16
	{
	public:

		// Do your magic here

		// Big 4
		MATHLIBRARY_API Mat3();
		MATHLIBRARY_API Mat3 &operator = (const Mat3 &A);
		MATHLIBRARY_API Mat3(const Mat3 &tM);
		MATHLIBRARY_API ~Mat3();

		MATHLIBRARY_API Mat3(float m0, float m1, float m2,
							float m3, float m4, float m5,
							float m6, float m7, float m8);

		// Big 6
		MATHLIBRARY_API Mat3(Mat3 &&) = default;
		MATHLIBRARY_API Mat3 &operator=(Mat3 &&) = default;

		MATHLIBRARY_API Mat3(const Vec3 &tV0, const Vec3 &tV1, const Vec3 &tV2);

		MATHLIBRARY_API explicit Mat3(const Mat4 &m);
		MATHLIBRARY_API explicit Mat3(const enum Identity_enum);

		// Get
		MATHLIBRARY_API Vec3 get(const Row3 type) const;

		// Set 
		MATHLIBRARY_API void set(const Mat3 &mIn);
		MATHLIBRARY_API void set(const enum Identity_enum);
		MATHLIBRARY_API void set(const Row3 type, const Vec3 &V);
		MATHLIBRARY_API void set(const Vec3 &V0, const Vec3 &V1, const Vec3 &V2);

		// bracket operators	
		MATHLIBRARY_API float &operator[] (const enum m0_enum);
		MATHLIBRARY_API float &operator[] (const enum m1_enum);
		MATHLIBRARY_API float &operator[] (const enum m2_enum);

		MATHLIBRARY_API float &operator[] (const enum m4_enum);
		MATHLIBRARY_API float &operator[] (const enum m5_enum);
		MATHLIBRARY_API float &operator[] (const enum m6_enum);

		MATHLIBRARY_API float &operator[] (const enum m8_enum);
		MATHLIBRARY_API float &operator[] (const enum m9_enum);
		MATHLIBRARY_API float &operator[] (const enum m10_enum);

		MATHLIBRARY_API float operator[] (const enum m0_enum) const;
		MATHLIBRARY_API float operator[] (const enum m1_enum) const;
		MATHLIBRARY_API float operator[] (const enum m2_enum) const;

		MATHLIBRARY_API float operator[] (const enum m4_enum) const;
		MATHLIBRARY_API float operator[] (const enum m5_enum) const;
		MATHLIBRARY_API float operator[] (const enum m6_enum) const;

		MATHLIBRARY_API float operator[] (const enum m8_enum) const;
		MATHLIBRARY_API float operator[] (const enum m9_enum) const;
		MATHLIBRARY_API float operator[] (const enum m10_enum) const;

		// Accessor	
		MATHLIBRARY_API void m0(const float v);
		MATHLIBRARY_API void m1(const float v);
		MATHLIBRARY_API void m2(const float v);

		MATHLIBRARY_API void m4(const float v);
		MATHLIBRARY_API void m5(const float v);
		MATHLIBRARY_API void m6(const float v);

		MATHLIBRARY_API void m8(const float v);
		MATHLIBRARY_API void m9(const float v);
		MATHLIBRARY_API void m10(const float v);

		MATHLIBRARY_API float m0() const;
		MATHLIBRARY_API float m1() const;
		MATHLIBRARY_API float m2() const;

		MATHLIBRARY_API float m4() const;
		MATHLIBRARY_API float m5() const;
		MATHLIBRARY_API float m6() const;

		MATHLIBRARY_API float m8() const;
		MATHLIBRARY_API float m9() const;
		MATHLIBRARY_API float m10() const;

		// Determinant
		MATHLIBRARY_API float det() const;

		// Transpose
		MATHLIBRARY_API Mat3 &T(void);
		MATHLIBRARY_API Mat3 getT(void)const;

		// Inverse
		MATHLIBRARY_API Mat3 getInv(void) const;
		MATHLIBRARY_API Mat3 &inv(void);

		// Comparison
		MATHLIBRARY_API bool isEqual(const Mat3 &A, const float epsilon = MATH_TOLERANCE) const;
		MATHLIBRARY_API bool isIdentity(const float epsilon = MATH_TOLERANCE) const;

		// Add operators
		MATHLIBRARY_API Mat3 operator + (void) const;
		MATHLIBRARY_API Mat3 operator + (const Mat3 &A) const;
		MATHLIBRARY_API Mat3 &operator += (const Mat3 &A);

		// Sub operators
		MATHLIBRARY_API Mat3 operator - (void) const;
		MATHLIBRARY_API Mat3 operator - (const Mat3 &A) const;
		MATHLIBRARY_API Mat3 &operator -= (const Mat3 &A);

		// Scale operators
		MATHLIBRARY_API Mat3 operator * (const float s) const;
		MATHLIBRARY_API friend Mat3 operator *(const float scale, const Mat3 &A);
		MATHLIBRARY_API Mat3 &operator *= (const float scale);

		// Multiply
		MATHLIBRARY_API Mat3 operator * (const Mat3 &A) const;
		MATHLIBRARY_API Mat3 &operator *= (const Mat3 &A);

		// For printing
		MATHLIBRARY_API void Print(const char *pName) const;

	private:

		friend Mat4;
		friend Vec3;
		friend Vec4;

		union
		{
			/*     | m0  m1  m2   0 | */
			/* m = | m4  m5  m6   0 | */
			/*     | m8  m9  m10  0 | */
			/*     | 0   0   0    1 | */

			struct
			{
				Vec4 _rows[4];
			};


			struct
			{
				Vec4 _v0;
				Vec4 _v1;
				Vec4 _v2;
				Vec4 _v3;
			};

			struct
			{
				float _m0;
				float _m1;
				float _m2;
				float _m3;
				float _m4;
				float _m5;
				float _m6;
				float _m7;
				float _m8;
				float _m9;
				float _m10;
				float _m11;
				float _m12;
				float _m13;
				float _m14;
				float _m15;
			};
		};
	};
}

#endif

//--- End of File ---
