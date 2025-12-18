//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "Mat4Hint.h"

namespace Azul
{
	// Do your magic here
	Mat4::Mat4()
	{
		this->_v0._mv = _mm_setzero_ps();
		this->_v1._mv = _mm_setzero_ps();
		this->_v2._mv = _mm_setzero_ps();
		this->_v3._mv = _mm_setzero_ps();
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

	Mat4& Mat4::operator = (const Quat& q)
	{
		this->set(q);
		return *this;
	}

	Mat4::Mat4(const Vec4& tV0, const Vec4& tV1, const Vec4& tV2, const Vec4& tV3)
	{
		this->_v0 = tV0;
		this->_v1 = tV1;
		this->_v2 = tV2;
		this->_v3 = tV3;
	}

	Mat4::Mat4(const Quat& q)
	{
		this->_v0._mv = _mm_set_ps(
			0,
			2.f * (q._qx * q._qz - q._qw * q._qy),
			2.f * (q._qx * q._qy + q._qw * q._qz),
			1.f - 2.f * (q._qy * q._qy + q._qz * q._qz)
		);

		this->_v1._mv = _mm_set_ps(
			0,
			2.f * (q._qy * q._qz + q._qw * q._qx),
			1.f - 2.f * (q._qx * q._qx + q._qz * q._qz),
			2.f * (q._qx * q._qy - q._qw * q._qz)
		);

		this->_v2._mv = _mm_set_ps(
			0,
			1.f - 2.f * (q._qx * q._qx + q._qy * q._qy),
			2.f * (q._qy * q._qz - q._qw * q._qx),
			2.f * (q._qx * q._qz + q._qw * q._qy)
		);

		this->_v3.set(0, 0, 0, 1);

		Mat4Hint::SetRotHint(*this);
	}

	Mat4::Mat4(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
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
			assert(false);
			break;
		}

		this->_u_m15 &= (unsigned int)(0xFFFFFFF8);
	}

	void Mat4::set(const Quat& q)
	{
		this->_v0._mv = _mm_set_ps(
			0,
			2.f * (q._qx * q._qz - q._qw * q._qy),
			2.f * (q._qx * q._qy + q._qw * q._qz),
			1.f - 2.f * (q._qy * q._qy + q._qz * q._qz)
		);

		this->_v1._mv = _mm_set_ps(
			0.f,
			2.f * (q._qy * q._qz + q._qw * q._qx),
			1.f - 2.f * (q._qx * q._qx + q._qz * q._qz),
			2.f * (q._qx * q._qy - q._qw * q._qz)
		);

		this->_v2._mv = _mm_set_ps(
			0,
			1.f - 2.f * (q._qx * q._qx + q._qy * q._qy),
			2.f * (q._qy * q._qz - q._qw * q._qx),
			2.f * (q._qx * q._qz + q._qw * q._qy)
		);

		this->_v3.set(0.f, 0.f, 0.f, 1.f);

		Mat4Hint::SetRotHint(*this);
	}

	void Mat4::set(const enum Identity_enum)
	{
		Mat4Hint::Hint h = Mat4Hint::GetMatHint(*this);

		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);

		switch (h)
		{
		case Azul::Mat4Hint::Hint::Rot:
			Mat4Hint::SetRotHint(*this);
			break;
		case Azul::Mat4Hint::Hint::Trans:
			Mat4Hint::SetTransHint(*this);
			break;
		case Azul::Mat4Hint::Hint::Scale:
			Mat4Hint::SetScaleHint(*this);
			break;
		case Azul::Mat4Hint::Hint::Generalize:
		case Azul::Mat4Hint::Hint::RotTrans:
		case Azul::Mat4Hint::Hint::RotScale:
		case Azul::Mat4Hint::Hint::TransScale:
		case Azul::Mat4Hint::Hint::Affine:
		default:
			Mat4Hint::SetGeneralHint(*this);
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
			assert(false);
			break;
		}

		return Vec4();
	}

	Mat4Proxy Mat4::operator[] (const enum m0_enum)
	{
		return Mat4Proxy(*this, this->_m0);
	}

	Mat4Proxy Mat4::operator[] (const enum m1_enum)
	{
		return Mat4Proxy(*this, this->_m1);
	}
	Mat4Proxy Mat4::operator[] (const enum m2_enum)
	{
		return Mat4Proxy(*this, this->_m2);
	}
	Mat4Proxy Mat4::operator[] (const enum m3_enum)
	{
		return Mat4Proxy(*this, this->_m3);
	}
	Mat4Proxy Mat4::operator[] (const enum m4_enum)
	{
		return Mat4Proxy(*this, this->_m4);
	}
	Mat4Proxy Mat4::operator[] (const enum m5_enum)
	{
		return Mat4Proxy(*this, this->_m5);
	}
	Mat4Proxy Mat4::operator[] (const enum m6_enum)
	{
		return Mat4Proxy(*this, this->_m6);
	}
	Mat4Proxy Mat4::operator[] (const enum m7_enum)
	{
		return Mat4Proxy(*this, this->_m7);
	}
	Mat4Proxy Mat4::operator[] (const enum m8_enum)
	{
		return Mat4Proxy(*this, this->_m8);
	}
	Mat4Proxy Mat4::operator[] (const enum m9_enum)
	{
		return Mat4Proxy(*this, this->_m9);
	}
	Mat4Proxy Mat4::operator[] (const enum m10_enum)
	{
		return Mat4Proxy(*this, this->_m10);
	}
	Mat4Proxy Mat4::operator[] (const enum m11_enum)
	{
		return Mat4Proxy(*this, this->_m11);
	}
	Mat4Proxy Mat4::operator[] (const enum m12_enum)
	{
		return Mat4Proxy(*this, this->_m12);
	}
	Mat4Proxy Mat4::operator[] (const enum m13_enum)
	{
		return Mat4Proxy(*this, this->_m13);
	}
	Mat4Proxy Mat4::operator[] (const enum m14_enum)
	{
		return Mat4Proxy(*this, this->_m14);
	}
	Mat4Proxy Mat4::operator[] (const enum m15_enum)
	{
		return Mat4Proxy(*this, this->_m15);
	}
	float Mat4::operator[] (const enum m0_enum) const
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
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m1(const float v)
	{
		this->_m1 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m2(const float v)
	{
		this->_m2 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m3(const float v)
	{
		this->_m3 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m4(const float v)
	{
		this->_m4 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m5(const float v)
	{
		this->_m5 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m6(const float v)
	{
		this->_m6 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m7(const float v)
	{
		this->_m7 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m8(const float v)
	{
		this->_m8 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m9(const float v)
	{
		this->_m9 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m10(const float v)
	{
		this->_m10 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m11(const float v)
	{
		this->_m11 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m12(const float v)
	{
		this->_m12 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m13(const float v)
	{
		this->_m13 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m14(const float v)
	{
		this->_m14 = v;
		Mat4Hint::ClearHint(*this);
	}

	void Mat4::m15(const float v)
	{
		this->_m15 = v;
		Mat4Hint::ClearHint(*this);
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
		float result = 0;


		Mat4Hint::Hint h = Mat4Hint::GetMatHint(*this);

		switch (h)
		{
		case Mat4Hint::Hint::Generalize:
		{
			float a0 = 0.f, a1 = 0.f, a2 = 0.f, a3 = 0.f;

			a0 = _m0 * (_m5 * (_m10 * _m15 - _m11 * _m14) -
				_m6 * (_m9 * _m15 - _m11 * _m13) +
				_m7 * (_m9 * _m14 - _m10 * _m13));

			a1 = _m1 * (_m4 * (_m10 * _m15 - _m11 * _m14) -
				_m6 * (_m8 * _m15 - _m11 * _m12) +
				_m7 * (_m8 * _m14 - _m10 * _m12));

			a2 = _m2 * (_m4 * (_m9 * _m15 - _m11 * _m13) -
				_m5 * (_m8 * _m15 - _m11 * _m12) +
				_m7 * (_m8 * _m13 - _m9 * _m12));

			a3 = _m3 * (_m4 * (_m9 * _m14 - _m10 * _m13) -
				_m5 * (_m8 * _m14 - _m10 * _m12) +
				_m6 * (_m8 * _m13 - _m9 * _m12));

			result = a0 - a1 + a2 - a3;
			break;
		}

		case Mat4Hint::Hint::Affine:
		{
			result = _m0 * (_m5 * _m10 - _m6 * _m9)
				- _m1 * (_m4 * _m10 - _m6 * _m8)
				+ _m2 * (_m4 * _m9 - _m5 * _m8);
			break;
		}
		case Mat4Hint::Hint::Rot:
		{
			Mat3 m(*this);

			result = _m0 * (_m5 * _m10 - _m6 * _m9) -
				_m1 * (_m4 * _m10 - _m6 * _m8) +
				_m2 * (_m4 * _m9 - _m5 * _m8);

			break;
		}
		case Mat4Hint::Hint::Trans:
		{
			result = 1.0f;
			break;
		}
		case Mat4Hint::Hint::RotTrans:
		{
			result = _m0 * (_m5 * _m10 - _m6 * _m9)
				- _m1 * (_m4 * _m10 - _m6 * _m8)
				+ _m2 * (_m4 * _m9 - _m5 * _m8);
			break;
		}
		case Mat4Hint::Hint::Scale:
		{
			result = _m0 * _m5 * _m10;
			break;
		}
		case Mat4Hint::Hint::RotScale:
		{
			result = _m0 * (_m5 * _m10 - _m6 * _m9)
				- _m1 * (_m4 * _m10 - _m6 * _m8)
				+ _m2 * (_m4 * _m9 - _m5 * _m8);
			break;
		}
		case Mat4Hint::Hint::TransScale:
		{
			result = _m0 * (_m5 * _m10 - _m6 * _m9)
				- _m1 * (_m4 * _m10 - _m6 * _m8)
				+ _m2 * (_m4 * _m9 - _m5 * _m8);
			break;
		}
		default:
			assert(false);
			break;
		}

		return result;
	}

	Mat4& Mat4::T(void)
	{
		std::swap(_m1, _m4);
		std::swap(_m2, _m8);
		std::swap(_m3, _m12);
		std::swap(_m6, _m9);
		std::swap(_m7, _m13);
		std::swap(_m11, _m14);
		Mat4Hint::Hint h = Mat4Hint::GetMatHint(*this);

		switch (h)
		{
		case Mat4Hint::Hint::Generalize:
		case Mat4Hint::Hint::Rot:
		case Mat4Hint::Hint::Scale:
			break;
		case Mat4Hint::Hint::Affine:
		case Mat4Hint::Hint::Trans:
		case Mat4Hint::Hint::RotTrans:
		case Mat4Hint::Hint::TransScale:
			// Be Generalize
			Mat4Hint::SetGeneralHint(*this);
			break;
		case Mat4Hint::Hint::RotScale:
			Mat4Hint::SetRotScaleHint(*this);
			break;
		default:
			break;
		}

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
		Mat4 inv(*this);


		Mat4Hint::Hint h = Mat4Hint::GetMatHint(*this);

		switch (h)
		{
		case Mat4Hint::Hint::Generalize:
		{
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
			break;
		}
		case Mat4Hint::Hint::Rot:
		{
			inv._m0 = _m0;  inv._m1 = _m4;  inv._m2 = _m8;
			inv._m4 = _m1;  inv._m5 = _m5;  inv._m6 = _m9;
			inv._m8 = _m2;  inv._m9 = _m6;  inv._m10 = _m10;

			break;
		}
		case Mat4Hint::Hint::Trans:
		{
			inv._m12 = -_m12;
			inv._m13 = -_m13;
			inv._m14 = -_m14;

			break;
		}
		case Mat4Hint::Hint::RotTrans:
		{
			inv._m0 = _m0;  inv._m1 = _m4;  inv._m2 = _m8;
			inv._m4 = _m1;  inv._m5 = _m5;  inv._m6 = _m9;
			inv._m8 = _m2;  inv._m9 = _m6;  inv._m10 = _m10;

			inv._m12 = -(_m12 * inv._m0 + _m13 * inv._m4 + _m14 * inv._m8);
			inv._m13 = -(_m12 * inv._m1 + _m13 * inv._m5 + _m14 * inv._m9);
			inv._m14 = -(_m12 * inv._m2 + _m13 * inv._m6 + _m14 * inv._m10);

			break;
		}
		case Mat4Hint::Hint::Scale:
		{
			inv._m0 = 1 / _m0;
			inv._m5 = 1 / _m5;
			inv._m10 = 1 / _m10;

			break;
		}
		case Mat4Hint::Hint::RotScale:
		{
			float sx = _v0.len();
			float sy = _v1.len();
			float sz = _v2.len();

			inv._m0 = _m0 / (sx * sx);
			inv._m1 = _m4 / (sy * sy);
			inv._m2 = _m8 / (sz * sz);

			inv._m4 = _m1 / (sx * sx);
			inv._m5 = _m5 / (sy * sy);
			inv._m6 = _m9 / (sz * sz);

			inv._m8 = _m2 / (sx * sx);
			inv._m9 = _m6 / (sy * sy);
			inv._m10 = _m10 / (sz * sz);

			break;
		}
		case Mat4Hint::Hint::TransScale:
		{
			inv._m0 = 1 / _m0;
			inv._m5 = 1 / _m5;
			inv._m10 = 1 / _m10;

			inv._m12 = -_m12 / _m0;
			inv._m13 = -_m13 / _m5;
			inv._m14 = -_m14 / _m10;

			break;
		}
		case Mat4Hint::Hint::Affine:
		{
			/* Slow
			{

			Mat3 A(*this);
			Vec3 C(_m12, _m13, _m14);

			Mat3 Ainv = Mat3(*this).getInv();
			// X Y
			// Z W

			// W = (D - C*A^-1*B) ^-1 = 1

			//X = (A - B*D^-1*C)^-1

			//Y = - A^-1 * B * scalarInv

			// Z = - scalarInv * C * A^-1
			Vec3 Z = -1 * C * Ainv;

			inv._v0.set(Ainv._v0.getVec3(), 0);
			inv._v1.set(Ainv._v1.getVec3(), 0);
			inv._v2.set(Ainv._v2.getVec3(), 0);

			inv._v3.set(Z, 1);
			}*/

			float det = _m0 * (_m5 * _m10 - _m6 * _m9)
				- _m1 * (_m4 * _m10 - _m6 * _m8)
				+ _m2 * (_m4 * _m9 - _m5 * _m8);

			assert(fabs(det) > 1e-6f);
			float invDet = 1.0f / det;

			inv._m0 = (_m5 * _m10 - _m6 * _m9) * invDet;
			inv._m1 = -(_m1 * _m10 - _m2 * _m9) * invDet;
			inv._m2 = (_m1 * _m6 - _m2 * _m5) * invDet;

			inv._m4 = -(_m4 * _m10 - _m6 * _m8) * invDet;
			inv._m5 = (_m0 * _m10 - _m2 * _m8) * invDet;
			inv._m6 = -(_m0 * _m6 - _m2 * _m4) * invDet;
			inv._m7 = 0.0f;

			inv._m8 = (_m4 * _m9 - _m5 * _m8) * invDet;
			inv._m9 = -(_m0 * _m9 - _m1 * _m8) * invDet;
			inv._m10 = (_m0 * _m5 - _m1 * _m4) * invDet;

			inv._m12 = -(_m12 * inv._m0 + _m13 * inv._m4 + _m14 * inv._m8);
			inv._m13 = -(_m12 * inv._m1 + _m13 * inv._m5 + _m14 * inv._m9);
			inv._m14 = -(_m12 * inv._m2 + _m13 * inv._m6 + _m14 * inv._m10);

			break;
		}
		default:
			assert(false);
			break;
		}

		Mat4Hint::SetHint(inv, h);

		return inv;
	}

	Mat4& Mat4::inv(void)
	{
		*this = this->getInv();
		return *this;
	}

	bool Mat4::isEqual(const Mat4& A, const float epsilon) const
	{
		return Util::isEqual(_m0, A._m0, epsilon) &&
			Util::isEqual(_m1, A._m1, epsilon) &&
			Util::isEqual(_m2, A._m2, epsilon) &&
			Util::isEqual(_m3, A._m3, epsilon) &&
			Util::isEqual(_m4, A._m4, epsilon) &&
			Util::isEqual(_m5, A._m5, epsilon) &&
			Util::isEqual(_m6, A._m6, epsilon) &&
			Util::isEqual(_m7, A._m7, epsilon) &&
			Util::isEqual(_m8, A._m8, epsilon) &&
			Util::isEqual(_m9, A._m9, epsilon) &&
			Util::isEqual(_m10, A._m10, epsilon) &&
			Util::isEqual(_m11, A._m11, epsilon) &&
			Util::isEqual(_m12, A._m12, epsilon) &&
			Util::isEqual(_m13, A._m13, epsilon) &&
			Util::isEqual(_m14, A._m14, epsilon) &&
			Util::isEqual(_m15, A._m15, epsilon);
	}

	bool Mat4::isIdentity(const float epsilon) const
	{
		return this->isEqual(Mat4(Identity), epsilon);
	}

	bool Mat4::isRotation(const float epsilon) const
	{
		Mat4Hint::Hint h = Mat4Hint::GetMatHint(*this);

		return h == Mat4Hint::Hint::Rot;
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
		return Mat4(this->_v0 * s, this->_v1 * s, this->_v2 * s, this->_v3 * s);
	}

	//what are u looking for?
	Mat4 operator*(const float scale, const Mat4& A)
	{
		return A * scale;
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

		Mat4Hint::SetNewHint(result, *this, A);

		return result;
	}

	Mat4& Mat4::operator*=(const Mat4& A)
	{
		*this = *this * A;
		return *this;
	}

	Mat4 Mat4::operator * (const Scale& A) const
	{
		Mat4 result(*this);

		result._m0 = _m0 * A._m0;
		result._m4 = _m4 * A._m0;
		result._m8 = _m8 * A._m0;
		result._m12 = _m12 * A._m0;

		result._m1 = _m1 * A._m5;
		result._m5 = _m5 * A._m5;
		result._m9 = _m9 * A._m5;
		result._m13 = _m13 * A._m5;

		result._m2 = _m2 * A._m10;
		result._m6 = _m6 * A._m10;
		result._m10 = _m10 * A._m10;
		result._m14 = _m14 * A._m10;

		//result.privSetNewHint(this->privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);

		return result;
	}

	Mat4& Mat4::operator *= (const Scale& A)
	{
		this->_m0 = _m0 * A._m0;
		this->_m4 = _m4 * A._m0;
		this->_m8 = _m8 * A._m0;
		this->_m12 = _m12 * A._m0;

		this->_m1 = _m1 * A._m5;
		this->_m5 = _m5 * A._m5;
		this->_m9 = _m9 * A._m5;
		this->_m13 = _m13 * A._m5;

		this->_m2 = _m2 * A._m10;
		this->_m6 = _m6 * A._m10;
		this->_m10 = _m10 * A._m10;
		this->_m14 = _m14 * A._m10;

		//privSetNewHint(privGetHint(), A.privGetHint());

		Mat4Hint::SetNewHint(*this, *this, A);

		return *this;
	}

	Mat4 Mat4::operator * (const Rot& A) const
	{
		Mat4 result;

		result._m0 = _m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8;
		result._m1 = _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9;
		result._m2 = _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10;
		result._m3 = _m3;

		result._m4 = _m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8;
		result._m5 = _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9;
		result._m6 = _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10;
		result._m7 = _m7;

		result._m8 = _m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8;
		result._m9 = _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9;
		result._m10 = _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10;
		result._m11 = _m11;

		result._m12 = _m12 * A._m0 + _m13 * A._m4 + _m14 * A._m8;
		result._m13 = _m12 * A._m1 + _m13 * A._m5 + _m14 * A._m9;
		result._m14 = _m12 * A._m2 + _m13 * A._m6 + _m14 * A._m10;
		result._m15 = _m15;

		//result.privSetNewHint(this->privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Mat4& Mat4::operator *= (const Rot& A)
	{
		float t0 = _m0, t1 = _m1, t2 = _m2, t3 = _m3;
		float t4 = _m4, t5 = _m5, t6 = _m6, t7 = _m7;
		float t8 = _m8, t9 = _m9, t10 = _m10, t11 = _m11;
		float t12 = _m12, t13 = _m13, t14 = _m14, t15 = _m15;

		_m0 = t0 * A._m0 + t1 * A._m4 + t2 * A._m8;
		_m1 = t0 * A._m1 + t1 * A._m5 + t2 * A._m9;
		_m2 = t0 * A._m2 + t1 * A._m6 + t2 * A._m10;
		_m3 = t3;

		_m4 = t4 * A._m0 + t5 * A._m4 + t6 * A._m8;
		_m5 = t4 * A._m1 + t5 * A._m5 + t6 * A._m9;
		_m6 = t4 * A._m2 + t5 * A._m6 + t6 * A._m10;
		_m7 = t7;

		_m8 = t8 * A._m0 + t9 * A._m4 + t10 * A._m8;
		_m9 = t8 * A._m1 + t9 * A._m5 + t10 * A._m9;
		_m10 = t8 * A._m2 + t9 * A._m6 + t10 * A._m10;
		_m11 = t11;

		_m12 = t12 * A._m0 + t13 * A._m4 + t14 * A._m8;
		_m13 = t12 * A._m1 + t13 * A._m5 + t14 * A._m9;
		_m14 = t12 * A._m2 + t13 * A._m6 + t14 * A._m10;
		_m15 = t15;

		//this->privSetNewHint(this->privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(*this, *this, A);
		return *this;
	}

	Mat4 Mat4::operator * (const Trans& A) const
	{
		Mat4 result(*this);

		result._m0 = _m0 + _m3 * A._m12;
		result._m1 = _m1 + _m3 * A._m13;
		result._m2 = _m2 + _m3 * A._m14;

		result._m4 = _m4 + _m7 * A._m12;
		result._m5 = _m5 + _m7 * A._m13;
		result._m6 = _m6 + _m7 * A._m14;

		result._m8 = _m8 + _m11 * A._m12;
		result._m9 = _m9 + _m11 * A._m13;
		result._m10 = _m10 + _m11 * A._m14;

		result._m12 = _m12 + _m15 * A._m12;
		result._m13 = _m13 + _m15 * A._m13;
		result._m14 = _m14 + _m15 * A._m14;

		//result.privSetNewHint(this->privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Mat4& Mat4::operator *= (const Trans& A)
	{
		this->_m0 = _m0 + _m3 * A._m12;
		this->_m1 = _m1 + _m3 * A._m13;
		this->_m2 = _m2 + _m3 * A._m14;

		this->_m4 = _m4 + _m7 * A._m12;
		this->_m5 = _m5 + _m7 * A._m13;
		this->_m6 = _m6 + _m7 * A._m14;

		this->_m8 = _m8 + _m11 * A._m12;
		this->_m9 = _m9 + _m11 * A._m13;
		this->_m10 = _m10 + _m11 * A._m14;

		this->_m12 = _m12 + _m15 * A._m12;
		this->_m13 = _m13 + _m15 * A._m13;
		this->_m14 = _m14 + _m15 * A._m14;

		//privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(*this, *this, A);
		return *this;
	}

	Mat4 Mat4::operator * (const Quat& q) const
	{
		return *this * Rot(q);
	}

	Mat4& Mat4::operator *= (const Quat& q)
	{
		*this *= Rot(q);
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
}


// ---  End of File ---
