//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here

	Mat4::Mat4()
	{
		this->_v0 = Vec4(0, 0, 0, 0);
		this->_v1 = Vec4(0, 0, 0, 0);
		this->_v2 = Vec4(0, 0, 0, 0);
		this->_v3 = Vec4(0, 0, 0, 0);
	}

	Mat4& Mat4::operator=(const Mat4& A)
	{
		if (this != &A)
		{
			for (int i = 0; i < 4; i++)
			{
				this->_rows[i] = A._rows[i];
			}
		}

		return *this;
	}

	Mat4::Mat4(const Mat4& tM)
	{
		for (int i = 0; i < 4; i++)
		{
			this->_rows[i] = tM._rows[i];
		}
	}

	Mat4::~Mat4()
	{
	}

	Mat4::Mat4(const Vec4& tV0, const Vec4& tV1, const Vec4& tV2, const Vec4& tV3)
	{
		this->_v0 = tV0;
		this->_v1 = tV1;
		this->_v2 = tV2;
		this->_v3 = tV3;
	}

	void Mat4::set(const Mat4& mIn)
	{
		*this = Mat4(mIn);
	}

	void Mat4::set(const Vec4& V0, const Vec4& V1, const Vec4& V2, const Vec4& V3)
	{
		this->_v0 = V0;
		this->_v1 = V1;
		this->_v2 = V2;
		this->_v3 = V3;
	}

	void Mat4::set(const Row4 type, const Vec4& V)
	{
		switch (type)
		{
		case Row4::i0:
			this->_v0 = V;
			break;
		case Row4::i1:
			this->_v1 = V;
			break;
		case Row4::i2:
			this->_v2 = V;
			break;
		case Row4::i3:
			this->_v3 = V;
			break;
		default:
			break;
		}
	}

	Vec4 Mat4::get(const Row4 type) const
	{
		switch (type)
		{
		case Row4::i0:
			return this->_v0;
			break;
		case Row4::i1:
			return this->_v1;
			break;
		case Row4::i2:
			return this->_v2;
			break;
		case Row4::i3:
			return this->_v3;
			break;
		default:
			break;
		}
		return Vec4();
	}

	float& Mat4::operator[] (const enum m0_enum)
	{
		return this->_m0;
	}

	float& Mat4::operator[] (const enum m1_enum)
	{
		return this->_m1;
	}

	float& Mat4::operator[] (const enum m2_enum)
	{
		return this->_m2;
	}

	float& Mat4::operator[] (const enum m3_enum)
	{
		return this->_m3;
	}
	float& Mat4::operator[] (const enum m4_enum)
	{
		return this->_m4;
	}
	float& Mat4::operator[] (const enum m5_enum)
	{
		return this->_m5;
	}
	float& Mat4::operator[] (const enum m6_enum)
	{
		return this->_m6;
	}
	float& Mat4::operator[] (const enum m7_enum)
	{
		return this->_m7;
	}
	float& Mat4::operator[] (const enum m8_enum)
	{
		return this->_m8;
	}
	float& Mat4::operator[] (const enum m9_enum)
	{
		return this->_m9;
	}
	float& Mat4::operator[] (const enum m10_enum)
	{
		return this->_m10;
	}
	float& Mat4::operator[] (const enum m11_enum)
	{
		return this->_m11;
	}
	float& Mat4::operator[] (const enum m12_enum)
	{
		return this->_m12;
	}
	float& Mat4::operator[] (const enum m13_enum)
	{
		return this->_m13;
	}
	float& Mat4::operator[] (const enum m14_enum)
	{
		return this->_m14;
	}
	float& Mat4::operator[] (const enum m15_enum)
	{
		return this->_m15;
	}

	void Mat4::set(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}

	Mat4::Mat4(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}

	float Mat4::operator[](const m0_enum) const
	{
		return this->_m0;
	}

	float Mat4::operator[](const m1_enum) const
	{
		return this->_m1;
	}

	float Mat4::operator[](const m2_enum) const
	{
		return this->_m2;
	}

	float Mat4::operator[](const m3_enum) const
	{
		return this->_m3;
	}

	float Mat4::operator[](const m4_enum) const
	{
		return this->_m4;
	}

	float Mat4::operator[](const m5_enum) const
	{
		return this->_m5;
	}

	float Mat4::operator[](const m6_enum) const
	{
		return this->_m6;
	}

	float Mat4::operator[](const m7_enum) const
	{
		return this->_m7;
	}

	float Mat4::operator[](const m8_enum) const
	{
		return this->_m8;
	}

	float Mat4::operator[](const m9_enum) const
	{
		return this->_m9;
	}

	float Mat4::operator[](const m10_enum) const
	{
		return this->_m10;
	}

	float Mat4::operator[](const m11_enum) const
	{
		return this->_m11;
	}

	float Mat4::operator[](const m12_enum) const
	{
		return this->_m12;
	}

	float Mat4::operator[](const m13_enum) const
	{
		return this->_m13;
	}

	float Mat4::operator[](const m14_enum) const
	{
		return this->_m14;
	}

	float Mat4::operator[](const m15_enum) const
	{
		return this->_m15;
	}

	void Mat4::m0(const float v)
	{
		this->_m0 = v;
	}

	void Mat4::m1(const float v)
	{
		this->_m1 = v;
	}

	void Mat4::m2(const float v)
	{
		this->_m2 = v;
	}

	void Mat4::m3(const float v)
	{
		this->_m3 = v;
	}

	void Mat4::m4(const float v)
	{
		this->_m4 = v;
	}

	void Mat4::m5(const float v)
	{
		this->_m5 = v;
	}

	void Mat4::m6(const float v)
	{
		this->_m6 = v;
	}

	void Mat4::m7(const float v)
	{
		this->_m7 = v;
	}

	void Mat4::m8(const float v)
	{
		this->_m8 = v;
	}

	void Mat4::m9(const float v)
	{
		this->_m9 = v;
	}

	void Mat4::m10(const float v)
	{
		this->_m10 = v;
	}

	void Mat4::m11(const float v)
	{
		this->_m11 = v;
	}

	void Mat4::m12(const float v)
	{
		this->_m12 = v;
	}

	void Mat4::m13(const float v)
	{
		this->_m13 = v;
	}

	void Mat4::m14(const float v)
	{
		this->_m14 = v;
	}

	void Mat4::m15(const float v)
	{
		this->_m15 = v;
	}

	float Mat4::m0() const
	{
		return this->_m0;
	}
	float Mat4::m1() const
	{
		return this->_m1;
	}
	float Mat4::m2() const
	{
		return this->_m2;
	}
	float Mat4::m3() const
	{
		return this->_m3;
	}
	float Mat4::m4() const
	{
		return this->_m4;
	}
	float Mat4::m5() const
	{
		return this->_m5;
	}
	float Mat4::m6() const
	{
		return this->_m6;
	}
	float Mat4::m7() const
	{
		return this->_m7;
	}
	float Mat4::m8() const
	{
		return this->_m8;
	}
	float Mat4::m9() const
	{
		return this->_m9;
	}
	float Mat4::m10() const
	{
		return this->_m10;
	}
	float Mat4::m11() const
	{
		return this->_m11;
	}
	float Mat4::m12() const
	{
		return this->_m12;
	}
	float Mat4::m13() const
	{
		return this->_m13;
	}
	float Mat4::m14() const
	{
		return this->_m14;
	}
	float Mat4::m15() const
	{
		return this->_m15;
	}
	float Mat4::det() const
	{
		float a0 = _m0 * (_m5 * (_m10 * _m15 - _m11 * _m14) -
			_m6 * (_m9 * _m15 - _m11 * _m13) +
			_m7 * (_m9 * _m14 - _m10 * _m13));

		float a1 = _m1 * (_m4 * (_m10 * _m15 - _m11 * _m14) -
			_m6 * (_m8 * _m15 - _m11 * _m12) +
			_m7 * (_m8 * _m14 - _m10 * _m12));

		float a2 = _m2 * (_m4 * (_m9 * _m15 - _m11 * _m13) -
			_m5 * (_m8 * _m15 - _m11 * _m12) +
			_m7 * (_m8 * _m13 - _m9 * _m12));

		float a3 = _m3 * (_m4 * (_m9 * _m14 - _m10 * _m13) -
			_m5 * (_m8 * _m14 - _m10 * _m12) +
			_m6 * (_m8 * _m13 - _m9 * _m12));

		return a0 - a1 + a2 - a3;
	}
	Mat4& Mat4::T(void)
	{
		std::swap(_m1, _m4);
		std::swap(_m2, _m8);
		std::swap(_m3, _m12);
		std::swap(_m6, _m9);
		std::swap(_m7, _m13);
		std::swap(_m11, _m14);

		return *this;
	}
	Mat4 Mat4::getT(void) const
	{
		Mat4 result;
		result._v0.set(_m0, _m4, _m8, _m12);
		result._v1.set(_m1, _m5, _m9, _m13);
		result._v2.set(_m2, _m6, _m10, _m14);
		result._v3.set(_m3, _m7, _m11, _m15);

		return result;
	}
	Mat4 Mat4::getInv(void) const
	{
		Mat4 inv;
		float det;

		inv._m0 = _m5 * _m10 * _m15 -
			_m5 * _m11 * _m14 -
			_m9 * _m6 * _m15 +
			_m9 * _m7 * _m14 +
			_m13 * _m6 * _m11 -
			_m13 * _m7 * _m10;

		inv._m1 = -_m1 * _m10 * _m15 +
			_m1 * _m11 * _m14 +
			_m9 * _m2 * _m15 -
			_m9 * _m3 * _m14 -
			_m13 * _m2 * _m11 +
			_m13 * _m3 * _m10;

		inv._m2 = _m1 * _m6 * _m15 -
			_m1 * _m7 * _m14 -
			_m5 * _m2 * _m15 +
			_m5 * _m3 * _m14 +
			_m13 * _m2 * _m7 -
			_m13 * _m3 * _m6;

		inv._m3 = -_m1 * _m6 * _m11 +
			_m1 * _m7 * _m10 +
			_m5 * _m2 * _m11 -
			_m5 * _m3 * _m10 -
			_m9 * _m2 * _m7 +
			_m9 * _m3 * _m6;

		inv._m4 = -_m4 * _m10 * _m15 +
			_m4 * _m11 * _m14 +
			_m8 * _m6 * _m15 -
			_m8 * _m7 * _m14 -
			_m12 * _m6 * _m11 +
			_m12 * _m7 * _m10;

		inv._m5 = _m0 * _m10 * _m15 -
			_m0 * _m11 * _m14 -
			_m8 * _m2 * _m15 +
			_m8 * _m3 * _m14 +
			_m12 * _m2 * _m11 -
			_m12 * _m3 * _m10;

		inv._m6 = -_m0 * _m6 * _m15 +
			_m0 * _m7 * _m14 +
			_m4 * _m2 * _m15 -
			_m4 * _m3 * _m14 -
			_m12 * _m2 * _m7 +
			_m12 * _m3 * _m6;

		inv._m7 = _m0 * _m6 * _m11 -
			_m0 * _m7 * _m10 -
			_m4 * _m2 * _m11 +
			_m4 * _m3 * _m10 +
			_m8 * _m2 * _m7 -
			_m8 * _m3 * _m6;

		inv._m8 = _m4 * _m9 * _m15 -
			_m4 * _m11 * _m13 -
			_m8 * _m5 * _m15 +
			_m8 * _m7 * _m13 +
			_m12 * _m5 * _m11 -
			_m12 * _m7 * _m9;

		inv._m9 = -_m0 * _m9 * _m15 +
			_m0 * _m11 * _m13 +
			_m8 * _m1 * _m15 -
			_m8 * _m3 * _m13 -
			_m12 * _m1 * _m11 +
			_m12 * _m3 * _m9;

		inv._m10 = _m0 * _m5 * _m15 -
			_m0 * _m7 * _m13 -
			_m4 * _m1 * _m15 +
			_m4 * _m3 * _m13 +
			_m12 * _m1 * _m7 -
			_m12 * _m3 * _m5;

		inv._m11 = -_m0 * _m5 * _m11 +
			_m0 * _m7 * _m9 +
			_m4 * _m1 * _m11 -
			_m4 * _m3 * _m9 -
			_m8 * _m1 * _m7 +
			_m8 * _m3 * _m5;

		inv._m12 = -_m4 * _m9 * _m14 +
			_m4 * _m10 * _m13 +
			_m8 * _m5 * _m14 -
			_m8 * _m6 * _m13 -
			_m12 * _m5 * _m10 +
			_m12 * _m6 * _m9;

		inv._m13 = _m0 * _m9 * _m14 -
			_m0 * _m10 * _m13 -
			_m8 * _m1 * _m14 +
			_m8 * _m2 * _m13 +
			_m12 * _m1 * _m10 -
			_m12 * _m2 * _m9;

		inv._m14 = -_m0 * _m5 * _m14 +
			_m0 * _m6 * _m13 +
			_m4 * _m1 * _m14 -
			_m4 * _m2 * _m13 -
			_m12 * _m1 * _m6 +
			_m12 * _m2 * _m5;

		inv._m15 = _m0 * _m5 * _m10 -
			_m0 * _m6 * _m9 -
			_m4 * _m1 * _m10 +
			_m4 * _m2 * _m9 +
			_m8 * _m1 * _m6 -
			_m8 * _m2 * _m5;

		det = _m0 * inv._m0 + _m1 * inv._m4 + _m2 * inv._m8 + _m3 * inv._m12;

		assert(det != 0.0f);

		float invDet = 1.0f / det;
		for (int i = 0; i < 16; i++)
			((&inv._m0)[i]) *= invDet;

		return inv;
	}

	Mat4& Mat4::inv(void)
	{
		*this = this->getInv();
		return *this;
	}

	bool Mat4::isEqual(const Mat4& A, const float epsilon) const
	{
		return std::fabs(_m0 - A._m0) <= epsilon &&
			std::fabs(_m1 - A._m1) <= epsilon &&
			std::fabs(_m2 - A._m2) <= epsilon &&
			std::fabs(_m3 - A._m3) <= epsilon &&
			std::fabs(_m4 - A._m4) <= epsilon &&
			std::fabs(_m5 - A._m5) <= epsilon &&
			std::fabs(_m6 - A._m6) <= epsilon &&
			std::fabs(_m7 - A._m7) <= epsilon &&
			std::fabs(_m8 - A._m8) <= epsilon &&
			std::fabs(_m9 - A._m9) <= epsilon &&
			std::fabs(_m10 - A._m10) <= epsilon &&
			std::fabs(_m11 - A._m11) <= epsilon &&
			std::fabs(_m12 - A._m12) <= epsilon &&
			std::fabs(_m13 - A._m13) <= epsilon &&
			std::fabs(_m14 - A._m14) <= epsilon &&
			std::fabs(_m15 - A._m15) <= epsilon;
	}

	bool Mat4::isIdentity(const float epsilon) const
	{
		return this->isEqual(Mat4(Identity));
	}

	bool Mat4::isRotation(const float epsilon) const
	{
		return false;
	}
	Mat4 Mat4::operator+(void) const
	{
		return *this;
	}
	Mat4 Mat4::operator+(const Mat4& A) const
	{
		Mat4 result = Mat4(*this);
		result._v0 += A._v0;
		result._v1 += A._v1;
		result._v2 += A._v2;
		result._v3 += A._v3;
		return result;
	}
	Mat4& Mat4::operator+=(const Mat4& A)
	{
		// TODO: insert return statement here
		*this = *this + A;
		return *this;
	}
	Mat4 Mat4::operator-(void) const
	{
		return Mat4(-this->_v0, -this->_v1, -this->_v2, -this->_v3);
	}

	Mat4 Mat4::operator-(const Mat4& A) const
	{
		Mat4 result = Mat4(*this);
		result._v0 -= A._v0;
		result._v1 -= A._v1;
		result._v2 -= A._v2;
		result._v3 -= A._v3;

		return result;
	}

	Mat4& Mat4::operator-=(const Mat4& A)
	{
		*this = *this - A;
		return *this;
	}

	Mat4 Mat4::operator*(const float s) const
	{
		return Mat4(this->_v0 * s, this->_v1 * s,this->_v2 * s, this->_v3 * s);
	}

	Mat4& Mat4::operator*=(const float scale)
	{
		this->_v0 *= scale;
		this->_v1 *= scale;
		this->_v2 *= scale;
		this->_v3 *= scale;

		return *this;
	}

	Mat4 Mat4::operator*(const Mat4& A) const
	{
		Mat4 result;

		result._m0 = _m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8 + _m3 * A._m12;
		result._m1 = _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9 + _m3 * A._m13;
		result._m2 = _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10 + _m3 * A._m14;
		result._m3 = _m0 * A._m3 + _m1 * A._m7 + _m2 * A._m11 + _m3 * A._m15;

		result._m4 = _m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8 + _m7 * A._m12;
		result._m5 = _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9 + _m7 * A._m13;
		result._m6 = _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10 + _m7 * A._m14;
		result._m7 = _m4 * A._m3 + _m5 * A._m7 + _m6 * A._m11 + _m7 * A._m15;

		result._m8 = _m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8 + _m11 * A._m12;
		result._m9 = _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9 + _m11 * A._m13;
		result._m10 = _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10 + _m11 * A._m14;
		result._m11 = _m8 * A._m3 + _m9 * A._m7 + _m10 * A._m11 + _m11 * A._m15;

		result._m12 = _m12 * A._m0 + _m13 * A._m4 + _m14 * A._m8 + _m15 * A._m12;
		result._m13 = _m12 * A._m1 + _m13 * A._m5 + _m14 * A._m9 + _m15 * A._m13;
		result._m14 = _m12 * A._m2 + _m13 * A._m6 + _m14 * A._m10 + _m15 * A._m14;
		result._m15 = _m12 * A._m3 + _m13 * A._m7 + _m14 * A._m11 + _m15 * A._m15;

		return result;
	}

	Mat4& Mat4::operator*=(const Mat4& A)
	{
		*this = *this * A;
		return *this;
	}
	void Mat4::Print(const char* pName) const
	{
		printf("(Mat4 %s =)\n", pName);
		printf("[ % .4f  % .4f  % .4f  % .4f ]\n", _m0, _m1, _m2, _m3);
		printf("[ % .4f  % .4f  % .4f  % .4f ]\n", _m4, _m5, _m6, _m7);
		printf("[ % .4f  % .4f  % .4f  % .4f ]\n", _m8, _m9, _m10, _m11);
		printf("[ % .4f  % .4f  % .4f  % .4f ]\n", _m12, _m13, _m14, _m15);
	}
	Mat4 operator*(const float s, const Mat4& A)
	{
		return Mat4(A._v0 * s, A._v1 * s, A._v2 * s, A._v3 * s);
	}
}

// ---  End of File ---
