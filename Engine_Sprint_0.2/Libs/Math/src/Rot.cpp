//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "Mat4Hint.h"

namespace Azul
{
	// Do your magic here
	Rot::Rot()
	{
		*this = Rot(Identity);
		Mat4Hint::SetRotHint(*this);
	}

	Rot::Rot(const Quat& q)
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

		this->_m15 = 1;

		Mat4Hint::SetRotHint(*this);
	}

	Rot& Rot::operator=(const Quat& q)
	{
		this->_v0.set(
			1.f - 2.f * (q._qy * q._qy + q._qz * q._qz),
			2.f * (q._qx * q._qy + q._qw * q._qz),
			2.f * (q._qx * q._qz - q._qw * q._qy),
			0.f);

		this->_v1.set(
			2.f * (q._qx * q._qy - q._qw * q._qz),
			1.f - 2.f * (q._qx * q._qx + q._qz * q._qz),
			2.f * (q._qy * q._qz + q._qw * q._qx),
			0.f
		);

		this->_v2.set(
			2.f * (q._qx * q._qz + q._qw * q._qy),
			2.f * (q._qy * q._qz - q._qw * q._qx),
			1.f - 2.f * (q._qx * q._qx + q._qy * q._qy),
			0.f
		);

		this->_v3.set(0, 0, 0, 1);

		Mat4Hint::SetRotHint(*this);
		return *this;
	}

	Rot::Rot(const Mat4& inM)
	{
		for (int i = 0; i < 4; i++)
		{
			this->_rows[i] = inM._rows[i];
		}
		Mat4Hint::SetRotHint(*this);
	}

	Rot::Rot(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);

		Mat4Hint::SetRotHint(*this);
	}

	Rot::Rot(const Rot1 type, const float angle)
	{
		const float cosA = Trig::cos(angle);
		const float sinA = Trig::sin(angle);

		switch (type)
		{
		case Azul::Rot1::X:
			this->_v0.set(1, 0, 0, 0);
			this->_v1.set(0, cosA, sinA, 0);
			this->_v2.set(0, -sinA, cosA, 0);
			this->_v3.set(0, 0, 0, 1);
			break;
		case Azul::Rot1::Y:
			this->_v0.set(cosA, 0, -sinA,0);
			this->_v1.set(0, 1, 0.0f, 0);
			this->_v2.set(sinA, 0, cosA, 0);
			this->_v3.set(0, 0, 0.0f, 1);
			break;

		case Azul::Rot1::Z:
			this->_v0.set(cosA, sinA, 0, 0);
			this->_v1.set(-sinA, cosA, 0, 0);
			this->_v2.set(0, 0, 1, 0);
			this->_v3.set(0, 0, 0, 1);
			break;
		default:
			break;
		}
		Mat4Hint::SetRotHint(*this);
	}

	Rot::Rot(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
	{
		switch (mode)
		{
		case Azul::Rot3::XYZ:
			*this = Rot(Rot1::X, angle_0) * Rot(Rot1::Y, angle_1) * Rot(Rot1::Z, angle_2);
			break;
		case Azul::Rot3::XZY:
			*this = Rot(Rot1::X, angle_0) * Rot(Rot1::Z, angle_2) * Rot(Rot1::Y, angle_1);
			break;
		case Azul::Rot3::YXZ:
			*this = Rot(Rot1::Y, angle_1) * Rot(Rot1::X, angle_0) * Rot(Rot1::Z, angle_2);
			break;
		case Azul::Rot3::YZX:
			*this = Rot(Rot1::Y, angle_1) * Rot(Rot1::Z, angle_2) * Rot(Rot1::X, angle_0);
			break;
		case Azul::Rot3::ZXY:
			*this = Rot(Rot1::Z, angle_2) * Rot(Rot1::X, angle_0) * Rot(Rot1::Y, angle_1);
			break;
		case Azul::Rot3::ZYX:
			*this = Rot(Rot1::Z, angle_2) * Rot(Rot1::Y, angle_1) * Rot(Rot1::X, angle_0);
			break;
		default:
			break;
		}
	}
	Rot::Rot(const Axis mode, const Vec3& vAxis, const float angle_radians)
	{
		Vec3 axis;
		switch (mode)
		{
		case Azul::Axis::AxisAngle:
			axis = vAxis.getNorm();
			break;
		case Azul::Axis::UnitAxisAngle:
			axis = vAxis;
			break;
		default:
			assert(false && "Invalid Axis mode");
			break;
		}

		const float x = axis._vx;
		const float y = axis._vy;
		const float z = axis._vz;

		const float c = Trig::cos(angle_radians);
		const float s = Trig::sin(angle_radians);
		const float t = 1.0f - c;

		this->_v0.set(
			t * x * x + c,
			t * x * y + s * z,
			t * x * z - s * y,
			0.0f
		);

		this->_v1.set(
			t * x * y - s * z,
			t * y * y + c,
			t * y * z + s * x,
			0.0f
		);

		this->_v2.set(
			t * x * z + s * y,
			t * y * z - s * x,
			t * z * z + c,
			0.0f
		);

		this->_v3.set(
			0.0f, 0.0f, 0.0f, 1.0f
		);

		Mat4Hint::SetRotHint(*this);
	}

	Rot::Rot(const Orient type, const Vec3& dof, const Vec3& up)
	{
		Vec3 vDir = dof.getNorm();       // z
		Vec3 vUpTemp = up.getNorm();
		Vec3 vRight = vUpTemp.cross(vDir).getNorm(); // x
		Vec3 vTrueUp = vDir.cross(vRight);           // y

		switch (type)
		{
		case Azul::Orient::LocalToWorld:
			this->_v0.set(vRight._vx, vRight._vy, vRight._vz, 0.0f);
			this->_v1.set(vTrueUp._vx, vTrueUp._vy, vTrueUp._vz, 0.0f);
			this->_v2.set(vDir._vx, vDir._vy, vDir._vz, 0.0f);
			this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
			break;

		case Azul::Orient::WorldToLocal:
			this->_v0.set(vRight._vx, vTrueUp._vx, vDir._vx, 0.0f);
			this->_v1.set(vRight._vy, vTrueUp._vy, vDir._vy, 0.0f);
			this->_v2.set(vRight._vz, vTrueUp._vz, vDir._vz, 0.0f);
			this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}

		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const Quat& q)
	{
		this->_v0.set(
			1.f - 2.f * (q._qy * q._qy + q._qz * q._qz),
			2.f * (q._qx * q._qy + q._qw * q._qz),
			2.f * (q._qx * q._qz - q._qw * q._qy),
			0.f);

		this->_v1.set(
			2.f * (q._qx * q._qy - q._qw * q._qz),
			1.f - 2.f * (q._qx * q._qx + q._qz * q._qz),
			2.f * (q._qy * q._qz + q._qw * q._qx),
			0.f
		);

		this->_v2.set(
			2.f * (q._qx * q._qz + q._qw * q._qy),
			2.f * (q._qy * q._qz - q._qw * q._qx),
			1.f - 2.f * (q._qx * q._qx + q._qy * q._qy),
			0.f
		);

		this->_v3.set(0.f, 0.f, 0.f, 1.f);

		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const enum Identity_enum)
	{
		this->_v0.set(1.f, 0.f, 0.f, 0.f);
		this->_v1.set(0.f, 1.f, 0.f, 0.f);
		this->_v2.set(0.f, 0.f, 1.f, 0.f);
		this->_v3.set(0.f, 0.f, 0.f, 1.f);

		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const Rot& inM)
	{
		for (int i = 0; i < 4; i++)
		{
			this->_rows[i] = inM._rows[i];
		}
		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const Rot1 type, const float angle)
	{
		const float cosA = Trig::cos(angle);
		const float sinA = Trig::sin(angle);

		switch (type)
		{
		case Azul::Rot1::X:
			this->_v0.set(1.f, 0.f, 0.f, 0.f);
			this->_v1.set(0.f, cosA, sinA, 0.f);
			this->_v2.set(0.f, -sinA, cosA, 0.f);
			this->_v3.set(0.f, 0.f, 0.f, 1.f);
			break;

		case Azul::Rot1::Y:
			this->_v0.set(cosA, 0.f, -sinA, 0.f);
			this->_v1.set(0.f, 1.f, 0.f, 0.f);
			this->_v2.set(sinA, 0.f, cosA, 0.f);
			this->_v3.set(0.f, 0.f, 0.f, 1.f);
			break;

		case Azul::Rot1::Z:
			this->_v0.set(cosA, sinA, 0.f, 0.f);
			this->_v1.set(-sinA, cosA, 0.f, 0.f);
			this->_v2.set(0.f, 0.f, 1.f, 0.f);
			this->_v3.set(0.f, 0.f, 0.f, 1.f);
			break;
		default:
			break;
		}
		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const Rot3 mode, const float angle_0, const float angle_1, const float angle_2)
	{
		switch (mode)
		{
		case Azul::Rot3::XYZ:
			*this = Rot(Rot1::X, angle_0) * Rot(Rot1::Y, angle_1) * Rot(Rot1::Z, angle_2);
			break;
		case Azul::Rot3::XZY:
			*this = Rot(Rot1::X, angle_0) * Rot(Rot1::Z, angle_2) * Rot(Rot1::Y, angle_1);
			break;
		case Azul::Rot3::YXZ:
			*this = Rot(Rot1::Y, angle_1) * Rot(Rot1::X, angle_0) * Rot(Rot1::Z, angle_2);
			break;
		case Azul::Rot3::YZX:
			*this = Rot(Rot1::Y, angle_1) * Rot(Rot1::Z, angle_2) * Rot(Rot1::X, angle_0);
			break;
		case Azul::Rot3::ZXY:
			*this = Rot(Rot1::Z, angle_2) * Rot(Rot1::X, angle_0) * Rot(Rot1::Y, angle_1);
			break;
		case Azul::Rot3::ZYX:
			*this = Rot(Rot1::Z, angle_2) * Rot(Rot1::Y, angle_1) * Rot(Rot1::X, angle_0);
			break;
		default:
			break;
		}
		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const Axis mode, const Vec3& vAxis, const float angle_radians)
	{
		Vec3 axis;
		switch (mode)
		{
		case Azul::Axis::AxisAngle:
			axis = vAxis.getNorm();
			break;
		case Azul::Axis::UnitAxisAngle:
			axis = vAxis;
			break;
		default:
			assert(false && "Invalid Axis mode");
			break;
		}

		const float x = axis._vx;
		const float y = axis._vy;
		const float z = axis._vz;

		const float c = Trig::cos(angle_radians);
		const float s = Trig::sin(angle_radians);
		const float t = 1.0f - c;

		this->_v0.set(
			t * x * x + c,
			t * x * y + s * z,
			t * x * z - s * y,
			0.0f);

		this->_v1.set(
			t * x * y - s * z,
			t * y * y + c,
			t * y * z + s * x,
			0.0f);

		this->_v2.set(
			t * x * z + s * y,
			t * y * z - s * x,
			t * z * z + c,
			0.0f);

		this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
		Mat4Hint::SetRotHint(*this);
	}

	void Rot::set(const Orient type, const Vec3& dof, const Vec3& up)
	{
		Vec3 vDir = dof.getNorm();       // z
		Vec3 vUpTemp = up.getNorm();
		Vec3 vRight = vUpTemp.cross(vDir).getNorm(); // x
		Vec3 vTrueUp = vDir.cross(vRight);           // y

		switch (type)
		{
		case Azul::Orient::LocalToWorld:
			this->_v0.set(vRight._vx, vRight._vy, vRight._vz, 0.0f);
			this->_v1.set(vTrueUp._vx, vTrueUp._vy, vTrueUp._vz, 0.0f);
			this->_v2.set(vDir._vx, vDir._vy, vDir._vz, 0.0f);
			this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		case Azul::Orient::WorldToLocal:
			this->_v0.set(vRight._vx, vTrueUp._vx, vDir._vx, 0.0f);
			this->_v1.set(vRight._vy, vTrueUp._vy, vDir._vy, 0.0f);
			this->_v2.set(vRight._vz, vTrueUp._vz, vDir._vz, 0.0f);
			this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}
		Mat4Hint::SetRotHint(*this);
	}

	Rot Rot::operator * (const Quat& A) const
	{
		return *this * Rot(A);
	}

	Rot& Rot::operator *= (const Quat& A)
	{
		*this = *this * Rot(A);
		return *this;
	}

	Mat4 Rot::operator * (const Mat4& A) const
	{
		Mat4 result;

		result._m0 = _m0 * A._m0 + _m1 * A._m4 + _m2 * A._m8;
		result._m1 = _m0 * A._m1 + _m1 * A._m5 + _m2 * A._m9;
		result._m2 = _m0 * A._m2 + _m1 * A._m6 + _m2 * A._m10;
		result._m3 = _m0 * A._m3 + _m1 * A._m7 + _m2 * A._m11 + _m3;

		result._m4 = _m4 * A._m0 + _m5 * A._m4 + _m6 * A._m8;
		result._m5 = _m4 * A._m1 + _m5 * A._m5 + _m6 * A._m9;
		result._m6 = _m4 * A._m2 + _m5 * A._m6 + _m6 * A._m10;
		result._m7 = _m4 * A._m3 + _m5 * A._m7 + _m6 * A._m11 + _m7;

		result._m8 = _m8 * A._m0 + _m9 * A._m4 + _m10 * A._m8;
		result._m9 = _m8 * A._m1 + _m9 * A._m5 + _m10 * A._m9;
		result._m10 = _m8 * A._m2 + _m9 * A._m6 + _m10 * A._m10;
		result._m11 = _m8 * A._m3 + _m9 * A._m7 + _m10 * A._m11 + _m11;

		result._m12 = A._m12;
		result._m13 = A._m13;
		result._m14 = A._m14;
		result._m15 = A._m15;

		//result.privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

	Mat4 Rot::operator*(const Scale& A) const
	{
		Mat4 result;

		result._m0 = _m0 * A._m0;
		result._m1 = _m1 * A._m5;
		result._m2 = _m2 * A._m10;

		result._m4 = _m4 * A._m0;
		result._m5 = _m5 * A._m5;
		result._m6 = _m6 * A._m10;

		result._m8 = _m8 * A._m0;
		result._m9 = _m9 * A._m5;
		result._m10 = _m10 * A._m10;
		result._m15 = 1.0f;

		//result.privSetNewHint(privGetHint(), A.privGetHint());
		Mat4Hint::SetNewHint(result, *this, A);

		return result;
	}

	Rot Rot::operator * (const Rot& A) const
	{
		Rot result;

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

	Rot& Rot::operator*=(const Rot& A)
	{
		float r0 = _m0, r1 = _m1, r2 = _m2;
		float r4 = _m4, r5 = _m5, r6 = _m6;
		float r8 = _m8, r9 = _m9, r10 = _m10;

		_m0 = r0 * A._m0 + r1 * A._m4 + r2 * A._m8;
		_m1 = r0 * A._m1 + r1 * A._m5 + r2 * A._m9;
		_m2 = r0 * A._m2 + r1 * A._m6 + r2 * A._m10;

		_m4 = r4 * A._m0 + r5 * A._m4 + r6 * A._m8;
		_m5 = r4 * A._m1 + r5 * A._m5 + r6 * A._m9;
		_m6 = r4 * A._m2 + r5 * A._m6 + r6 * A._m10;

		_m8 = r8 * A._m0 + r9 * A._m4 + r10 * A._m8;
		_m9 = r8 * A._m1 + r9 * A._m5 + r10 * A._m9;
		_m10 = r8 * A._m2 + r9 * A._m6 + r10 * A._m10;

		return *this;
	}

	Mat4 Rot::operator*(const Trans& A) const
	{
		Mat4 result(*this);

		result._m12 = A._m12;
		result._m13 = A._m13;
		result._m14 = A._m14;

		Mat4Hint::SetNewHint(result, *this, A);
		return result;
	}

}

// ---  End of File ---
