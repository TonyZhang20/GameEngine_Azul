//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here

	Mat3::Mat3()
	{
		this->_v0._mv = _mm_setzero_ps();
		this->_v1._mv = _mm_setzero_ps();
		this->_v2._mv = _mm_setzero_ps();
		this->_v3._mv = _mm_set_ps(1, 0, 0, 0);
	}

	Mat3& Mat3::operator=(const Mat3& A)
	{
		if (this != &A)
		{
			for (int i = 0; i < 3; i++)
			{
				this->_rows[i] = A._rows[i];
			}
		}

		return *this;
	}

	Mat3::Mat3(const Mat3& tM)
	{
		for (int i = 0; i < 3; i++)
		{
			this->_rows[i] = tM._rows[i];
		}
	}

	Mat3::~Mat3()
	{
	}

	Mat3::Mat3(float m0, float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8)
	{
		this->_v0.set(m0, m1, m2, 0.0f);
		this->_v1.set(m3, m4, m5, 0.0f);
		this->_v2.set(m6, m7, m8, 0.0f);
		this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
	}

	Mat3::Mat3(const enum Identity_enum)
	{
		this->_v0.set(1.f, 0.f, 0.f, 0.f);
		this->_v1.set(0.f, 1.f, 0.f, 0.f);
		this->_v2.set(0.f, 0.f, 1.f, 0.f);
		this->_v3.set(0.f, 0.f, 0.f, 1.f);
	}

	Mat3::Mat3(const Vec3& tV0, const Vec3& tV1, const Vec3& tV2)
	{
		this->_v0.set(tV0, 0.f);
		this->_v1.set(tV1, 0.f);
		this->_v2.set(tV2, 0.f);
	}

	Vec3 Mat3::get(const Row3 type) const
	{
		switch (type)
		{
		case Row3::i0:
			return this->_v0.getVec3();
			break;
		case Row3::i1:
			return this->_v1.getVec3();
			break;
		case Row3::i2:
			return this->_v2.getVec3();
			break;
		default:
			break;
		}
		return Vec3();
	}

	void Mat3::set(const Mat3& mIn)
	{
		for (int i = 0; i < 3; i++)
		{
			this->_rows[0] = mIn._rows[0];
		}
	}

	void Mat3::set(const Row3 type, const Vec3& V)
	{
		switch (type)
		{
		case Row3::i0:
			return this->_v0.set(Vec4(V, 0.f));
			break;
		case Row3::i1:
			return this->_v1.set(Vec4(V, 0.f));
			break;
		case Row3::i2:
			return this->_v2.set(Vec4(V, 0.f));
			break;
		default:
			break;
		}
	}

	void Mat3::set(const Vec3& V0, const Vec3& V1, const Vec3& V2)
	{
		this->_v0.set(V0, 0.f);
		this->_v1.set(V1, 0.f);
		this->_v2.set(V2, 0.f);
	}

	float& Mat3::operator[] (const enum m0_enum)
	{
		return this->_m0;
	}

	float& Mat3::operator[] (const enum m1_enum)
	{
		return this->_m1;
	}

	float& Mat3::operator[] (const enum m2_enum)
	{
		return this->_m2;
	}

	float& Mat3::operator[] (const enum m4_enum)
	{
		return this->_m4;
	}

	float& Mat3::operator[] (const enum m5_enum)
	{
		return this->_m5;
	}

	float& Mat3::operator[] (const enum m6_enum)
	{
		return this->_m6;
	}

	float& Mat3::operator[] (const enum m8_enum)
	{
		return this->_m8;
	}

	float& Mat3::operator[] (const enum m9_enum)
	{
		return this->_m9;
	}

	float& Mat3::operator[] (const enum m10_enum)
	{
		return this->_m10;
	}

	Mat3::Mat3(const Mat4& m)
	{
		this->_v0.set(m._m0, m._m1, m._m2, 0.f);
		this->_v1.set(m._m4, m._m5, m._m6, 0.f);
		this->_v2.set(m._m8, m._m9, m._m10, 0.f);
		this->_v3.set(0.f, 0.f, 0.f, 1.f);
	}

	void Mat3::set(const enum Identity_enum)
	{
		this->_v0.set(1.f, 0.f, 0.f, 0.f);
		this->_v1.set(0.f, 1.f, 0.f, 0.f);
		this->_v2.set(0.f, 0.f, 1.f, 0.f);
		this->_v3.set(0.f, 0.f, 0.f, 1.f);
	}


	float Mat3::operator[](const m0_enum) const
	{
		return this->_m0;
	}

	float Mat3::operator[](const m1_enum) const
	{
		return this->_m1;
	}

	float Mat3::operator[](const m2_enum) const
	{
		return this->_m2;
	}

	float Mat3::operator[](const m4_enum) const
	{
		return this->_m4;
	}

	float Mat3::operator[](const m5_enum) const
	{
		return this->_m5;
	}

	float Mat3::operator[](const m6_enum) const
	{
		return this->_m6;
	}


	float Mat3::operator[](const m8_enum) const
	{
		return this->_m8;
	}

	float Mat3::operator[](const m9_enum) const
	{
		return this->_m9;
	}

	float Mat3::operator[](const m10_enum) const
	{
		return this->_m10;
	}

	void Mat3::m0(const float v)
	{
		this->_m0 = v;
	}

	void Mat3::m1(const float v)
	{
		this->_m1 = v;
	}

	void Mat3::m2(const float v)
	{
		this->_m2 = v;
	}

	void Mat3::m4(const float v)
	{
		this->_m4 = v;
	}

	void Mat3::m5(const float v)
	{
		this->_m5 = v;
	}

	void Mat3::m6(const float v)
	{
		this->_m6 = v;
	}

	void Mat3::m8(const float v)
	{
		this->_m8 = v;
	}

	void Mat3::m9(const float v)
	{
		this->_m9 = v;
	}

	void Mat3::m10(const float v)
	{
		this->_m10 = v;
	}

	float Mat3::m0() const
	{
		return this->_m0;
	}
	float Mat3::m1() const
	{
		return this->_m1;
	}
	float Mat3::m2() const
	{
		return this->_m2;
	}
	float Mat3::m4() const
	{
		return this->_m4;
	}
	float Mat3::m5() const
	{
		return this->_m5;
	}
	float Mat3::m6() const
	{
		return this->_m6;
	}
	float Mat3::m8() const
	{
		return this->_m8;
	}
	float Mat3::m9() const
	{
		return this->_m9;
	}
	float Mat3::m10() const
	{
		return this->_m10;
	}

	float Mat3::det() const
	{
		return
			_m0 * (_m5 * _m10 - _m6 * _m9) -
			_m1 * (_m4 * _m10 - _m6 * _m8) +
			_m2 * (_m4 * _m9 - _m5 * _m8);
	}

	Mat3& Mat3::T(void)
	{
		*this = this->getT();
		return *this;
	}

	Mat3 Mat3::getT(void) const
	{
		Mat3 result;

		result._m0 = this->_m0;
		result._m1 = this->_m4;
		result._m2 = this->_m8;

		result._m4 = this->_m1;
		result._m5 = this->_m5;
		result._m6 = this->_m9;

		result._m8 = this->_m2;
		result._m9 = this->_m6;
		result._m10 = this->_m10;

		return result;
	}

	Mat3 Mat3::getInv(void) const
	{
		float det = this->det();
		assert(std::fabs(det) > 1e-6f);

		float invDet = 1.0f / det;

		Mat3 result;

		result._m0 = (_m5 * _m10 - _m6 * _m9) * invDet;
		result._m1 = (_m2 * _m9 - _m1 * _m10) * invDet;
		result._m2 = (_m1 * _m6 - _m2 * _m5) * invDet;

		result._m4 = (_m6 * _m8 - _m4 * _m10) * invDet;
		result._m5 = (_m0 * _m10 - _m2 * _m8) * invDet;
		result._m6 = (_m2 * _m4 - _m0 * _m6) * invDet;

		result._m8 = (_m4 * _m9 - _m5 * _m8) * invDet;
		result._m9 = (_m1 * _m8 - _m0 * _m9) * invDet;
		result._m10 = (_m0 * _m5 - _m1 * _m4) * invDet;

		return result;
	}

	Mat3& Mat3::inv(void)
	{
		*this = this->getInv();
		return *this;
	}

	bool Mat3::isEqual(const Mat3& A, const float epsilon) const
	{
		return
			Util::isEqual(this->_m0, A._m0, epsilon) &&
			Util::isEqual(this->_m1, A._m1, epsilon) &&
			Util::isEqual(this->_m2, A._m2, epsilon) &&

			Util::isEqual(this->_m4, A._m4, epsilon) &&
			Util::isEqual(this->_m5, A._m5, epsilon) &&
			Util::isEqual(this->_m6, A._m6, epsilon) &&

			Util::isEqual(this->_m8, A._m8, epsilon) &&
			Util::isEqual(this->_m9, A._m9, epsilon) &&
			Util::isEqual(this->_m10, A._m10, epsilon);
	}

	bool Mat3::isIdentity(const float epsilon) const
	{
		return this->isEqual(Mat3(Identity));
	}

	Mat3 Mat3::operator+(void) const
	{
		return *this;
	}

	Mat3 Mat3::operator+(const Mat3& A) const
	{
		Mat3 result = Mat3(*this);
		result._v0 += A._v0;
		result._v1 += A._v1;
		result._v2 += A._v2;

		return result;
	}

	Mat3& Mat3::operator+=(const Mat3& A)
	{
		this->_v0 += A._v0;
		this->_v1 += A._v1;
		this->_v2 += A._v2;

		return *this;
	}

	Mat3 Mat3::operator-(void) const
	{
		return Mat3(Vec3(-this->_v0), Vec3(-this->_v1), Vec3(-this->_v2));
	}

	Mat3 Mat3::operator-(const Mat3& A) const
	{
		Mat3 result = Mat3(*this);
		result._v0 -= A._v0;
		result._v1 -= A._v1;
		result._v2 -= A._v2;

		return result;
	}

	Mat3& Mat3::operator-=(const Mat3& A)
	{
		this->_v0 -= A._v0;
		this->_v1 -= A._v1;
		this->_v2 -= A._v2;

		return *this;
	}

	Mat3 Mat3::operator*(const float s) const
	{
		return Mat3(Vec3(this->_v0 * s), Vec3(this->_v1 * s), Vec3(this->_v2 * s));
	}

	Mat3& Mat3::operator*=(const float scale)
	{
		this->_v0 *= scale;
		this->_v1 *= scale;
		this->_v2 *= scale;

		return *this;
	}

	Mat3 Mat3::operator*(const Mat3& A) const
	{
		Mat3 result;

		result._m0 = _m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8;
		result._m1 = _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9;
		result._m2 = _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10;

		result._m4 = _m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8;
		result._m5 = _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9;
		result._m6 = _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10;

		result._m8 = _m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8;
		result._m9 = _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9;
		result._m10 = _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10;

		return result;
	}

	Mat3& Mat3::operator*=(const Mat3& A)
	{
		*this = *this * A;
		return *this;
	}

	void Mat3::Print(const char* pName) const
	{
		printf("(Matrix: %s) \n", pName);
		printf("[ %.4f  %.4f  %.4f ]\n", _m0, _m1, _m2);
		printf("[ %.4f  %.4f  %.4f ]\n", _m4, _m5, _m6);
		printf("[ %.4f  %.4f  %.4f ]\n", _m8, _m9, _m10);
	}

	Mat3 operator*(const float scale, const Mat3& A)
	{
		return Mat3(Vec3(A._v0 * scale), Vec3(A._v1 * scale), Vec3(A._v2 * scale));
	}

}

// ---  End of File ---
