//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"

namespace Azul
{
	// Do your magic here



	Rot::Rot()
	{
		*this = Rot(Identity);
	}

	Rot::Rot(const Mat4& inM)
	{
		for (int i = 0; i < 4; i++)
		{
			this->_rows[i] = inM._rows[i];
		}
	}

	Rot::Rot(const Rot1 type, const float angle)
	{
		switch (type)
		{
		case Azul::Rot1::X:
			this->_v0.set(1, 0, 0, 0);
			this->_v1.set(0, cos(angle), sin(angle), 0);
			this->_v2.set(0, -sin(angle), cos(angle), 0);
			this->_v3.set(0, 0, 0, 1);
			break;
		case Azul::Rot1::Y:
			this->_v0.set(cos(angle), 0.0f, -sin(angle), 0.0f);
			this->_v1.set(0.0f, 1.0f, 0.0f, 0.0f);
			this->_v2.set(sin(angle), 0.0f, cos(angle), 0.0f);
			this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
			break;

		case Azul::Rot1::Z:
			this->_v0.set(cos(angle), sin(angle), 0.0f, 0.0f);
			this->_v1.set(-sin(angle), cos(angle), 0.0f, 0.0f);
			this->_v2.set(0.0f, 0.0f, 1.0f, 0.0f);
			this->_v3.set(0.0f, 0.0f, 0.0f, 1.0f);
			break;
		default:
			break;
		}
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

		const float c = cosf(angle_radians);
		const float s = sinf(angle_radians);
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
	}

	void Rot::set(const Rot& inM)
	{
		for (int i = 0; i < 4; i++)
		{
			this->_rows[i] = inM._rows[i];
		}
	}

	void Rot::set(const Rot1 type, const float angle)
	{
		switch (type)
		{
		case Azul::Rot1::X:
			this->_v0.set(1, 0, 0, 0);
			this->_v1.set(0, cos(angle), sin(angle), 0);
			this->_v2.set(0, -sin(angle), cos(angle), 0);
			this->_v3.set(0, 0, 0, 1);
			break;

		case Azul::Rot1::Y:
			this->_v0.set(cos(angle), 0, -sin(angle), 0);
			this->_v1.set(0, 1, 0, 0);
			this->_v2.set(sin(angle), 0, cos(angle), 0);
			this->_v3.set(0, 0, 0, 1);
			break;

		case Azul::Rot1::Z:
			this->_v0.set(cos(angle), sin(angle), 0, 0);
			this->_v1.set(-sin(angle), cos(angle), 0, 0);
			this->_v2.set(0, 0, 1, 0);
			this->_v3.set(0, 0, 0, 1);
			break;
		default:
			break;
		}
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

		const float c = cosf(angle_radians);
		const float s = sinf(angle_radians);
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
	}

	Rot::Rot(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}

	void Rot::set(const enum Identity_enum)
	{
		this->_v0.set(1, 0, 0, 0);
		this->_v1.set(0, 1, 0, 0);
		this->_v2.set(0, 0, 1, 0);
		this->_v3.set(0, 0, 0, 1);
	}

}

// ---  End of File ---
