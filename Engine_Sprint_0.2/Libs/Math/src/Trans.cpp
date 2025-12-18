//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "Mat4Hint.h"
namespace Azul
{
	// Do your magic here
	Trans::Trans()
	{
		this->set(Identity);
		Mat4Hint::SetTransHint(*this);
	}

	Trans::Trans(const enum Identity_enum)
	{
		this->set(Identity);
		Mat4Hint::SetTransHint(*this);
	}

	Trans::Trans(const float tx, const float ty, const float tz)
	{
		this->set(Identity);
		this->_v3._mv = _mm_set_ps(1.f, tz, ty, tx);

		Mat4Hint::SetTransHint(*this);
	}

	Trans::Trans(const Vec3& vTrans)
	{
		this->set(Identity);
		this->_v3._mv = _mm_set_ps(1.f, vTrans.z(), vTrans.y(), vTrans.x());

		Mat4Hint::SetTransHint(*this);
	}

	void Trans::set(const float tx, const float ty, const float tz)
	{
		this->set(Identity);

		this->_v3._mv = _mm_set_ps(1.f, tz, ty, tx);
		Mat4Hint::SetTransHint(*this);
	}

	void Trans::set(const Vec3& vTrans)
	{
		this->set(Identity);

		this->_v3._mv = _mm_set_ps(1.f, vTrans.z(), vTrans.y(), vTrans.x());
		Mat4Hint::SetTransHint(*this);
	}

	void Trans::set(const enum Identity_enum)
	{
		this->_v0.set(1.f, 0.f, 0.f, 0.f);
		this->_v1.set(0.f, 1.f, 0.f, 0.f);
		this->_v2.set(0.f, 0.f, 1.f, 0.f);
		this->_v3.set(0.f, 0.f, 0.f, 1.f);

		Mat4Hint::SetTransHint(*this);
	}

	Mat4 Trans::operator * (const Mat4& A) const
	{
		Mat4 result(A);

		result._m12 = _m12 * A._m0 + _m13 * A._m4 + _m14 * A._m8 + A._m12;
		result._m13 = _m12 * A._m1 + _m13 * A._m5 + _m14 * A._m9 + A._m13;
		result._m14 = _m12 * A._m2 + _m13 * A._m6 + _m14 * A._m10 + A._m14;
		result._m15 = _m12 * A._m3 + _m13 * A._m7 + _m14 * A._m11 + A._m15;

		//result.privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Mat4 Trans::operator * (const Scale& A) const
	{
		Mat4 result(A);

		result._m12 = _m12 * A._m0;
		result._m13 = _m13 * A._m5;
		result._m14 = _m14 * A._m10;
		result._m15 = 1;

		//result.privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Mat4 Trans::operator * (const Rot& A) const
	{
		Mat4 result(A);

		result._m12 += _m12 * A._m0 + _m13 * A._m4 + _m14 * A._m8;
		result._m13 += _m12 * A._m1 + _m13 * A._m5 + _m14 * A._m9;
		result._m14 += _m12 * A._m2 + _m13 * A._m6 + _m14 * A._m10;

		//result.privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Trans Trans::operator * (const Trans& A) const
	{
		Trans result;

		result._m12 = _m12 + A._m12;
		result._m13 = _m13 + A._m13;
		result._m14 = _m14 + A._m14;

		return result;
	}

	Trans& Trans::operator *= (const Trans& A)
	{
		_m12 += A._m12;
		_m13 += A._m13;
		_m14 += A._m14;

		return *this;
	}

	Mat4 Trans::operator * (const Quat& q) const
	{
		Mat4 result(q);

		result._m12 += _m12 * result._m0 + _m13 * result._m4 + _m14 * result._m8;
		result._m13 += _m12 * result._m1 + _m13 * result._m5 + _m14 * result._m9;
		result._m14 += _m12 * result._m2 + _m13 * result._m6 + _m14 * result._m10;

		((Mat4Hint&)result).privSetNewHint(Mat4Hint::Hint::Trans, Mat4Hint::Hint::Rot);

		return result;
	}

}

// ---  End of File ---
