//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "Mat4Hint.h"
namespace Azul
{
	// Do your magic here
	Scale::Scale()
	{
		this->set(Identity);
	}

	Scale::Scale(const float sx, const float sy, const float sz)
	{
		this->_m0 = sx;
		this->_m5 = sy;
		this->_m10 = sz;
		this->_m15 = 1;

		Mat4Hint::SetScaleHint(*this);
	}

	Scale::Scale(const Vec3& vScale)
	{
		this->_m0 = vScale.x();
		this->_m5 = vScale.y();
		this->_m10 = vScale.z();
		this->_m15 = 1;

		Mat4Hint::SetScaleHint(*this);
	}

	Scale::Scale(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);

		Mat4Hint::SetScaleHint(*this);
	}

	void Scale::set(const float sx, const float sy, const float sz)
	{
		this->_m0 = sx;
		this->_m5 = sy;
		this->_m10 = sz;
	}

	void Scale::set(const Vec3& vScale)
	{
		this->_m0 = vScale.x();
		this->_m5 = vScale.y();
		this->_m10 = vScale.z();
	}

	void Scale::set(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);

		Mat4Hint::SetScaleHint(*this);
	}

	Mat4 Scale::operator * (const Mat4& A) const
	{
		Mat4 result;

		result._m0 = _m0 * A._m0;
		result._m1 = _m0 * A._m1;
		result._m2 = _m0 * A._m2;
		result._m3 = _m0 * A._m3;

		result._m4 = _m5 * A._m4;
		result._m5 = _m5 * A._m5;
		result._m6 = _m5 * A._m6;
		result._m7 = _m5 * A._m7;

		result._m8 = _m10 * A._m8;
		result._m9 = _m10 * A._m9;
		result._m10 = _m10 * A._m10;
		result._m11 = _m10 * A._m11;

		result._m12 = A._m12;
		result._m13 = A._m13;
		result._m14 = A._m14;
		result._m15 = A._m15;

		//result.privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Scale Scale::operator * (const Scale& A) const
	{
		Scale result;
		result._m0 = _m0 * A._m0;
		result._m5 = _m5 * A._m5;
		result._m10 = _m10 * A._m10;

		return result;
	}

	Scale& Scale::operator *= (const Scale& A)
	{
		_m0 *= A._m0;
		_m5 *= A._m5;
		_m10 *= A._m10;

		return *this;
	}

	Mat4 Scale::operator * (const Rot& A) const
	{
		Mat4 result;

		result._m0 = _m0 * A._m0;
		result._m1 = _m0 * A._m1;
		result._m2 = _m0 * A._m2;

		result._m4 = _m5 * A._m4;
		result._m5 = _m5 * A._m5;
		result._m6 = _m5 * A._m6;

		result._m8 = _m10 * A._m8;
		result._m9 = _m10 * A._m9;
		result._m10 = _m10 * A._m10;

		result._m15 = 1.0f;

		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Mat4 Scale::operator * (const Trans& A) const
	{
		Mat4 result;

		result._m0 = _m0;
		result._m5 = _m5;
		result._m10 = _m10;

		result._m12 = A._m12;
		result._m13 = A._m13;
		result._m14 = A._m14;

		result._m15 = 1.0f;

		Mat4Hint::SetNewHint(result, *this, A);

		return result;
	}

	Mat4 Scale::operator * (const Quat& q) const
	{
		return *this * Rot(q);
	}


}

// ---  End of File ---
