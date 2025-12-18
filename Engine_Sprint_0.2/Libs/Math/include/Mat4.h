//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_MATRIX_4x4_H
#define ENGINE_MATH_MATRIX_4x4_H

#include "Math_DLLInterface.h"

#include "Vec4.h"
#include "Vec3.h"
#include "Enum.h"
// Leave this ON for final grading
#define MATH_USE_HINTS

namespace Azul
{
	struct Mat4Proxy;
	class Quat;

	class Mat4 : public Align16
	{

	public:

		// Do your magic here

		// Big 4
		MATH_LIBRARY_API Mat4();
		MATH_LIBRARY_API Mat4& operator = (const Mat4& A);
		MATH_LIBRARY_API Mat4(const Mat4&);
		MATH_LIBRARY_API ~Mat4();

		// Big 6
		MATH_LIBRARY_API Mat4(Mat4&&) = default;
		MATH_LIBRARY_API Mat4& operator = (Mat4&&) = default;
		MATH_LIBRARY_API Mat4& operator = (const Quat&);

		// Constructors
		MATH_LIBRARY_API Mat4(const Vec4& tV0, const Vec4& tV1, const Vec4& tV2, const Vec4& tV3);
		MATH_LIBRARY_API explicit Mat4(const enum Identity_enum);
		MATH_LIBRARY_API explicit Mat4(const Quat& q);

		// Set 
		MATH_LIBRARY_API void set(const Mat4& mIn);
		MATH_LIBRARY_API void set(const Vec4& V0, const Vec4& V1, const Vec4& V2, const Vec4& V3);
		MATH_LIBRARY_API void set(const enum Identity_enum);
		MATH_LIBRARY_API void set(Row4 type, const Vec4& V);
		MATH_LIBRARY_API void set(const Quat& q);

		// Get
		MATH_LIBRARY_API Vec4 get(const Row4 type) const;

		// bracket operators	
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m0_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m1_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m2_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m3_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m4_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m5_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m6_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m7_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m8_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m9_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m10_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m11_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m12_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m13_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m14_enum);
		MATH_LIBRARY_API Mat4Proxy operator[] (const enum m15_enum);

		MATH_LIBRARY_API float operator[] (const enum m0_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m1_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m2_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m3_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m4_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m5_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m6_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m7_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m8_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m9_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m10_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m11_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m12_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m13_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m14_enum) const;
		MATH_LIBRARY_API float operator[] (const enum m15_enum) const;

		// Accessor	
		MATH_LIBRARY_API void m0(const float v);
		MATH_LIBRARY_API void m1(const float v);
		MATH_LIBRARY_API void m2(const float v);
		MATH_LIBRARY_API void m3(const float v);
		MATH_LIBRARY_API void m4(const float v);
		MATH_LIBRARY_API void m5(const float v);
		MATH_LIBRARY_API void m6(const float v);
		MATH_LIBRARY_API void m7(const float v);
		MATH_LIBRARY_API void m8(const float v);
		MATH_LIBRARY_API void m9(const float v);
		MATH_LIBRARY_API void m10(const float v);
		MATH_LIBRARY_API void m11(const float v);
		MATH_LIBRARY_API void m12(const float v);
		MATH_LIBRARY_API void m13(const float v);
		MATH_LIBRARY_API void m14(const float v);
		MATH_LIBRARY_API void m15(const float v);

		MATH_LIBRARY_API float m0() const;
		MATH_LIBRARY_API float m1() const;
		MATH_LIBRARY_API float m2() const;
		MATH_LIBRARY_API float m3() const;
		MATH_LIBRARY_API float m4() const;
		MATH_LIBRARY_API float m5() const;
		MATH_LIBRARY_API float m6() const;
		MATH_LIBRARY_API float m7() const;
		MATH_LIBRARY_API float m8() const;
		MATH_LIBRARY_API float m9() const;
		MATH_LIBRARY_API float m10() const;
		MATH_LIBRARY_API float m11() const;
		MATH_LIBRARY_API float m12() const;
		MATH_LIBRARY_API float m13() const;
		MATH_LIBRARY_API float m14() const;
		MATH_LIBRARY_API float m15() const;

		// Determinant
		MATH_LIBRARY_API float det() const;

		// Transpose
		MATH_LIBRARY_API Mat4& T(void);
		MATH_LIBRARY_API Mat4 getT(void)const;

		// Inverse
		MATH_LIBRARY_API Mat4 getInv(void) const;
		MATH_LIBRARY_API Mat4& inv(void);

		// Comparison
		MATH_LIBRARY_API bool isEqual(const Mat4& A, const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isIdentity(const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isRotation(const float epsilon = MATH_TOLERANCE) const;

		// Add operators
		MATH_LIBRARY_API Mat4 operator + (void) const;
		MATH_LIBRARY_API Mat4 operator + (const Mat4& A) const;
		MATH_LIBRARY_API Mat4& operator += (const Mat4& A);

		// Sub operators
		MATH_LIBRARY_API Mat4 operator - (void) const;
		MATH_LIBRARY_API Mat4 operator - (const Mat4& A) const;
		MATH_LIBRARY_API Mat4& operator -= (const Mat4& A);

		// Scale operators
		MATH_LIBRARY_API Mat4 operator * (const float s) const;
		MATH_LIBRARY_API friend Mat4 operator *(const float scale, const Mat4& A);
		MATH_LIBRARY_API Mat4& operator *= (const float scale);

		// Multiply
		MATH_LIBRARY_API Mat4 operator * (const Mat4& A) const;
		MATH_LIBRARY_API Mat4& operator *= (const Mat4& A);

		// Simple casting to Mat4 operations... just a wrapper - with hints
		MATH_LIBRARY_API Mat4 operator * (const Scale& A) const;
		MATH_LIBRARY_API Mat4& operator *= (const Scale& A);

		MATH_LIBRARY_API Mat4 operator * (const Rot& A) const;
		MATH_LIBRARY_API Mat4& operator *= (const Rot& A);

		MATH_LIBRARY_API Mat4 operator * (const Trans& A) const;
		MATH_LIBRARY_API Mat4& operator *= (const Trans& A);

		MATH_LIBRARY_API Mat4 operator * (const Quat &q) const;
		MATH_LIBRARY_API Mat4 &operator *= (const Quat &q);

		// For printing
		MATH_LIBRARY_API void Print(const char* pName) const;

	protected:
		friend Mat3;
		friend Vec3;
		friend Vec4;
		friend Mat4Proxy;
		friend Scale;
		friend Trans;
		friend Rot;

		union
		{
			/*     | m0  m1  m2   m3  | */
			/* m = | m4  m5  m6   m7  | */
			/*     | m8  m9  m10  m11 | */
			/*     | m12 m13 m14  m15 | */

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
				union
				{
					float _m15;
					unsigned int _u_m15;
				};
			};
		};
	};
}

#endif

//--- End of File ---
