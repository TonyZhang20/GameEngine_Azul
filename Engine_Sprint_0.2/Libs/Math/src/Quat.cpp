//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#include "MathEngine.h"
#include "Mat4Hint.h"

#define FEqual(A, B, Dif) Util::isEqual(A, B, Dif)

namespace Azul
{

	// Do your magic here
	Quat::Quat(void)
	{
		this->_mq = _mm_set_ps(1.0f, 0.f, .0f, .0f);
	}

	Quat::Quat(const Quat& qIn)
	{
		this->_mq = qIn._mq;
	}

	Quat::Quat(const Vec3& vect, const float real)
	{
		this->_mq = _mm_set_ps(real, vect.z(), vect.y(), vect.x());
	}

	Quat::Quat(const float vx, const float vy, const float vz, const float real)
	{
		this->_mq = _mm_set_ps(real, vz, vy, vx);
	}

	Quat::Quat(const Rot& M)
	{
		this->set(Identity);

		float tr = M.m0() + M.m5() + M.m10();
		float S;

		if (tr > 0.0f)
		{
			S = 2.f * Trig::sqrt(tr + 1.0f);
			this->_mq = _mm_set_ps(
				0.25f * S, 					//w
				-(M.m4() - M.m1()) / S,		//z
				-(M.m2() - M.m8()) / S,		//y
				-(M.m9() - M.m6()) / S		//x
			);
		}
		else if ((M.m0() > M.m5() && (M.m0() > M.m10())))
		{
			S = 2.f * Trig::sqrt(1.0f + M.m0() - M.m5() - M.m10());
			this->_mq = _mm_set_ps(
				-(M.m9() - M.m6()) / S,
				(M.m2() + M.m8()) / S,
				(M.m1() + M.m4()) / S,
				0.25f * S
			);
		}
		else if (M.m5() > M.m10())
		{
			S = 2.f * Trig::sqrt(1.0f + M.m5() - M.m0() - M.m10());
			this->_mq = _mm_set_ps(
				-(M.m2() - M.m8()) / S,
				(M.m6() + M.m9()) / S,
				0.25f * S,
				(M.m1() + M.m4()) / S
			);
		}
		else
		{
			S = 2.f * Trig::sqrt(1.0f + M.m10() - M.m0() - M.m5());
			this->_mq = _mm_set_ps(
				-(M.m4() - M.m1()) / S,
				0.25f * S,
				(M.m6() + M.m9()) / S,
				(M.m2() + M.m8()) / S
			);
		}
	}

	Quat::Quat(const Rot1 type, const float angle)
	{
		this->set(Identity);

		switch (type)
		{
		case Azul::Rot1::X:
			this->_qx = Trig::sin(angle / 2);
			break;
		case Azul::Rot1::Y:
			this->_qy = Trig::sin(angle / 2);
			break;
		case Azul::Rot1::Z:
			this->_qz = Trig::sin(angle / 2);
			break;
		default:
			break;
		}

		this->_qw = Trig::cos(angle / 2);
	}

	Quat::Quat(const Rot3 type, const float angle_x, const float angle_y, const float angle_z)
	{
		this->set(Identity);
		this->set(type, angle_x, angle_y, angle_z);
	}

	Quat::Quat(const Axis type, const Vec3& vAxis, const float angle_radians)
	{
		this->set(Identity);
		this->set(type, vAxis, angle_radians);
	}

	Quat::Quat(const Orient ort, const Vec3& dof, const Vec3& up)
	{
		this->set(Identity);
		this->set(ort, dof, up);
	}

	Quat::~Quat(void)
	{

	}

	Quat::Quat(const enum Identity_enum)
	{
		this->_mq = _mm_set_ps(1.0f, 0.f, .0f, .0f);
	}

	float Quat::operator[](const x_enum value) const
	{
		return this->_qx;
	}
	float Quat::operator[](const y_enum value) const
	{
		return this->_qy;
	}
	float Quat::operator[](const z_enum value) const
	{
		return this->_qz;
	}
	float Quat::operator[](const w_enum value) const
	{
		return this->_qw;
	}

	void Quat::set(const enum Identity_enum)
	{
		this->_mq = _mm_set_ps(1.f, 0.f, 0.f, 0.f);
	}

	void Quat::set(const Quat& q)
	{
		this->_mq = q._mq;
	}

	void Quat::set(const Rot& q)
	{
		this->_mq = Quat(q)._mq;
	}

	void Quat::set(const Rot1 type, const float angle)
	{
		this->set(Identity);

		switch (type)
		{
		case Azul::Rot1::X:
			this->_qx = Trig::sin(angle / 2);
			break;
		case Azul::Rot1::Y:
			this->_qy = Trig::sin(angle / 2);
			break;
		case Azul::Rot1::Z:
			this->_qz = Trig::sin(angle / 2);
			break;
		default:
			break;
		}

		this->_qw = Trig::cos(angle / 2);
	}

	void Quat::set(const Rot3 type, const float angle_x, const float angle_y, const float angle_z)
	{
		this->set(Identity);

		switch (type)
		{
		case Azul::Rot3::XYZ:
			*this = Quat(Rot1::X, angle_x) * Quat(Rot1::Y, angle_y) * Quat(Rot1::Z, angle_z);
			break;
		case Azul::Rot3::XZY:
			*this = Quat(Rot1::X, angle_x) * Quat(Rot1::Z, angle_z) * Quat(Rot1::Y, angle_y);
			break;
		case Azul::Rot3::YXZ:
			*this = Quat(Rot1::Y, angle_y) * Quat(Rot1::X, angle_x) * Quat(Rot1::Z, angle_z);
			break;
		case Azul::Rot3::YZX:
			*this = Quat(Rot1::Y, angle_y) * Quat(Rot1::Z, angle_z) * Quat(Rot1::X, angle_x);
			break;
		case Azul::Rot3::ZXY:
			*this = Quat(Rot1::Z, angle_z) * Quat(Rot1::X, angle_x) * Quat(Rot1::Y, angle_y);
			break;
		case Azul::Rot3::ZYX:
			*this = Quat(Rot1::Z, angle_z) * Quat(Rot1::Y, angle_y) * Quat(Rot1::X, angle_x);
			break;
		default:
			break;
		}
	}

	void Quat::set(const Axis type, const Vec3& vAxis, const float angle_radians)
	{
		Vec3 unitAxis;

		switch (type)
		{
		case Axis::AxisAngle:
			unitAxis = vAxis.getNorm();
			break;
		case Axis::UnitAxisAngle:
			unitAxis = vAxis;
			break;
		default:
			break;
		}

		const float halfAngle = 0.5f * angle_radians;

		unitAxis *= Trig::sin(halfAngle);

		this->_qx = unitAxis.x();
		this->_qy = unitAxis.y();
		this->_qz = unitAxis.z();
		this->_qw = Trig::cos(halfAngle);

	}

	void Quat::set(const Orient ort, const Vec3& dof, const Vec3& up)
	{
		Rot rot(ort, dof, up);
		this->set(rot);
	}

	void Quat::set(const float vx, const float vy, const float vz, const float real)
	{
		this->_mq = _mm_set_ps(real, vz, vy, vx);
	}

	float& Quat::operator[](const x_enum value)
	{
		return this->_qx;
	}

	float& Quat::operator[](const y_enum value)
	{
		return this->_qy;
	}

	float& Quat::operator[](const z_enum value)
	{
		return this->_qz;
	}

	float& Quat::operator[](const w_enum value)
	{
		return this->_qw;
	}

	void Quat::qx(const float x)
	{
		this->_qx = x;
	}

	void Quat::qy(const float y)
	{
		this->_qy = y;
	}

	void Quat::qz(const float z)
	{
		this->_qz = z;
	}

	void Quat::qw(const float w)
	{
		this->_qw = w;
	}

	void Quat::real(const float r)
	{
		this->_real = r;
	}

	void Quat::setVec3(const Vec3& vect)
	{
		this->_mq = _mm_set_ps(this->_qw, vect.z(), vect.y(), vect.x());
	}

	void Quat::set(const Vec3& vect, const float real)
	{
		this->_mq = _mm_set_ps(real, vect.z(), vect.y(), vect.x());
	}

	float Quat::qx()const
	{
		return this->_qx;
	}

	float Quat::qy()const
	{
		return this->_qy;
	}

	float Quat::qz()const
	{
		return this->_qz;
	}

	float Quat::qw()const
	{
		return this->_qw;
	}

	float Quat::real()const
	{
		return this->_real;
	}

	float Quat::getAngle(void) const
	{
		return 2 * Trig::acos(_qw);
	}

	void Quat::getVec3(Vec3& vOut) const
	{
		vOut = _qV3;
	}

	void Quat::getAxis(Vec3& vOut) const
	{
		getVec3(vOut);
		vOut.norm();
	}

	float Quat::dot(const Quat& qin) const
	{
		__m128 tmp = _mm_dp_ps(this->_mq, qin._mq, 0xF1);
		float result = _mm_cvtss_f32(tmp);
		return result;
	}

	float Quat::mag(void) const
	{
		return Trig::sqrt(_qx * _qx + _qy * _qy + _qz * _qz + _qw * _qw);
	}

	float Quat::magSquared(void) const
	{
		return _qx * _qx + _qy * _qy + _qz * _qz + _qw * _qw;
	}

	float Quat::invMag(void) const
	{
		float m = mag();
		if (m > 0.f)
		{
			return 1.f / m;
		}

		return 0.f;
	}

	MATH_LIBRARY_API Vec3 Quat::Forward(void) const
	{
		return Vec3(
			2.0f * (_qx * _qz + _qw * _qy),
			2.0f * (_qy * _qz - _qw * _qx),
			1.0f - 2.0f * (_qx * _qx + _qy * _qy)
		);
	}

	MATH_LIBRARY_API Vec3 Quat::Right(void) const
	{
		return Vec3(
			1.0f - 2.0f * (_qy * _qy + _qz * _qz),
			2.0f * (_qx * _qy + _qw * _qz),
			2.0f * (_qx * _qz - _qw * _qy)
		);
	}

	MATH_LIBRARY_API Vec3 Quat::Up(void) const
	{
		return Vec3(
			2.0f * (_qx * _qy - _qw * _qz),
			1.0f - 2.0f * (_qx * _qx + _qz * _qz),
			2.0f * (_qy * _qz + _qw * _qx)
		);
	}

	Quat& Quat::conj(void)
	{
		this->_mq = _mm_set_ps(_qw, -_qz, -_qy, -_qx);
		return *this;
	}

	Quat Quat::getConj(void) const
	{
		return Quat( -_qx, -_qy, -_qz, _qw);
	}

	Quat& Quat::inv(void)
	{
		if (Util::isEqual(magSquared(), 1.0, MATH_TOLERANCE))
		{
			conj();
		}
		else
		{
			Quat conj = getConj();

			float invSqr = 1.0f / magSquared();

			conj = conj * invSqr;

			this->_mq = conj._mq;
		}

		return *this;
	}

	Quat Quat::getInv(void) const
	{
		if (FEqual(magSquared(), 1.0, MATH_TOLERANCE))
		{
			return getConj();
		}
		else
		{
			Quat conj = getConj();

			float invSqr = 1.0f / magSquared();

			conj = conj * invSqr;

			return conj;
		}
	}

	Quat& Quat::norm(void)
	{
		float len = mag();

		if (len == 0)
			this->_mq = _mm_set_ps(0, 0, 0, 0);
		else
			this->_mq = _mm_set_ps(_qw / len, _qz / len, _qy / len, _qx / len);

		return *this;
	}

	Quat Quat::getNorm(void) const
	{
		float len = mag();
		Quat r;

		if (len == 0)
			r._mq = _mm_set_ps(0, 0, 0, 0);
		else
			r._mq = _mm_set_ps(_qw / len, _qz / len, _qy / len, _qx / len);

		return r;	
	}

	void Quat::Lqvqc(const Vec3& vIn, Vec3& vOut) const //Rotation
	{
		// vOut = Quat * vIn * Quat.conj()
		
		Vec3 qv;
		this->getVec3(qv);

		vOut = 2 * _qw * vIn.cross(qv) + (_qw * _qw - qv.dot(qv)) * vIn + 2 * qv.dot(vIn) * qv;

	}

	void Quat::Lqcvq(const Vec3& vIn, Vec3& vOut) const //q* Rotation
	{
		// vOut = Quat.conj() * vIn * Quat		

		Vec3 qv;
		this->getVec3(qv);

		vOut = 2 * _qw * qv.cross(vIn) + (_qw * _qw - qv.dot(qv)) * vIn + 2 * qv.dot(vIn) * qv;
	}

	bool Quat::isEqual(const Quat& qin, const float epsilon) const
	{
		if (FEqual(this->_qx, qin._qx, epsilon) && FEqual(this->_qy, qin._qy, epsilon)
			&& FEqual(this->_qz, qin._qz, epsilon) && FEqual(this->_qw, qin._qw, epsilon))
		{
			return true;
		}

		return false;
	}

	bool Quat::isEquivalent(const Quat& qin, const float epsilon) const
	{
		return isEqual(qin, epsilon) || isNegEqual(qin, epsilon);
	}

	bool Quat::isNegEqual(const Quat& qin, const float epsilon) const
	{
		if (FEqual(this->_qx, -qin._qx, epsilon) && FEqual(this->_qy, -qin._qy, epsilon)
			&& FEqual(this->_qz, -qin._qz, epsilon) && FEqual(this->_qw, -qin._qw, epsilon))
		{
			return true;
		}

		return false;
	}

	bool Quat::isConjugateEqual(const Quat& qin, const float epsilon) const
	{
		return FEqual(this->_qx, -qin._qx, epsilon) &&
			FEqual(this->_qy, -qin._qy, epsilon) &&
			FEqual(this->_qz, -qin._qz, epsilon) &&
			FEqual(this->_qw, qin._qw, epsilon);
	}

	bool Quat::isIdentity(const float epsilon) const
	{
		return FEqual(this->_qx, 0.f, epsilon) &&
			FEqual(this->_qy, 0.f, epsilon) &&
			FEqual(this->_qz, 0.f, epsilon) &&
			FEqual(this->_qw, 1.f, epsilon);
	}

	bool Quat::isNormalized(const float epsilon) const
	{
		return FEqual(magSquared(), 1.0, epsilon);
	}
	
	Quat& Quat::operator=(const Quat& q)
	{
		this->set(q);
		return *this;
	}

	Quat& Quat::operator=(const Rot& m)
	{
		this->set(m);
		return *this;
	}

	Quat Quat::operator+(void) const
	{
		return *this;
	}

	Quat Quat::operator+(const Quat& q) const
	{
		return Quat(
			_qx + q._qx,
			_qy + q._qy,
			_qz + q._qz,
			_qw + q._qw
		);
	}

	Quat& Quat::operator+=(const Quat& q)
	{
		this->_mq = _mm_add_ps(this->_mq, q._mq);
		return *this;
	}

	Quat Quat::operator+(const float a) const
	{
		return Quat(
			_qx + a,
			_qy + a,
			_qz + a,
			_qw + a
		);
	}

	Quat& Quat::operator+=(const float a)
	{
		__m128 scalar = _mm_set1_ps(a);
		this->_mq = _mm_add_ps(this->_mq, scalar);

		return *this;
	}

	Quat operator+(const float a, const Quat& q)
	{
		return Quat(
			a + q._qx,
			a + q._qy,
			a + q._qz,
			a + q._qw
		);
	}

	Quat Quat::operator-(void) const
	{
		return Quat(-_qx, -_qy, -_qz, -_qw);
	}

	Quat Quat::operator-(const Quat& q) const
	{
		return Quat(_qx - q._qx, _qy - q._qy, _qz - q._qz, _qw - q._qw);
	}

	Quat& Quat::operator-=(const Quat& q)
	{
		this->_mq = _mm_sub_ps(this->_mq, q._mq);
		return *this;
	}

	Quat Quat::operator-(const float a) const
	{
		return Quat(
			_qx - a,
			_qy - a,
			_qz - a,
			_qw - a
		);
	}

	Quat& Quat::operator-=(const float a)
	{
		__m128 scalar = _mm_set1_ps(a);
		this->_mq = _mm_sub_ps(this->_mq, scalar);

		return *this;
	}

	Quat operator-(const float a, const Quat& q)
	{
		return Quat(
			a - q._qx,
			a - q._qy,
			a - q._qz,
			a - q._qw
		);
	}

	Quat Quat::operator*(const float a) const
	{
		return Quat(
			a * _qx,
			a * _qy,
			a * _qz,
			a * _qw
		);
	}

	Quat& Quat::operator*=(const float a)
	{
		__m128 scalar = _mm_set1_ps(a);
		this->_mq = _mm_mul_ps(this->_mq, scalar);

		return *this;
	}

	Quat operator*(const float a, const Quat& q)
	{
		return Quat(
			a * q._qx,
			a * q._qy,
			a * q._qz,
			a * q._qw
		);
	}

	Mat4 Quat::operator*(const Mat4& m) const
	{
		return Rot(*this) * m;
	}

	Quat Quat::operator*(const Quat& q) const
	{
		Vec3 pv, qv;
		this->getVec3(pv);
		q.getVec3(qv);

		return Quat( 
			_qw * qv + q._qw * pv - pv.cross(qv),
			_qw * q._qw - pv.dot(qv)
		);
	}

	Quat& Quat::operator*=(const Quat& q)
	{
		Vec3 pv, qv;
		this->getVec3(pv);
		q.getVec3(qv);

		this->set(
			_qw * qv + q._qw * pv - pv.cross(qv),
			_qw * q._qw - pv.dot(qv));
	
		return *this;
	}

	Mat4 Quat::operator*(const Scale& m) const
	{
		return Rot(*this) * m;
	}

	Quat& Quat::operator*=(const Rot& m)
	{
		*this *= Quat(m);
		return *this;
	}

	Rot Quat::operator*(const Rot& m) const
	{
		return Rot(*this) * m;
	}

	Mat4 Quat::operator*(const Trans& m) const
	{
		Mat4 result(*this);

		result[m12] = m[m12];
		result[m13] = m[m13];
		result[m14] = m[m14];
		
		Mat4Hint::SetNewHint(result, Mat4Hint::Hint::Rot, ((Mat4Hint&)m).GetHint());
		
		return result;
	}

	Quat Quat::operator/(const Quat& q) const
	{
		assert(q._qx != 0);
		assert(q._qy != 0);
		assert(q._qz != 0);
		assert(q._qw != 0);
		
		return Quat(
			_qx / q._qx,
			_qy / q._qy,
			_qz / q._qz,
			_qw / q._qw
		);
	}

	Quat& Quat::operator/=(const Quat& q)
	{
		this->_mq = _mm_div_ps(this->_mq, q._mq);
		return *this;
	}

	Quat Quat::operator/(const float a) const
	{
		//assert(a != 0);

		return Quat(
			_qx / a,
			_qy / a,
			_qz / a,
			_qw / a
		);
	}

	Quat& Quat::operator/=(const float a)
	{
		__m128 scalar = _mm_set1_ps(a);
		this->_mq = _mm_div_ps(this->_mq, scalar);
		return *this;
	}

	Quat operator/(const float a, const Quat& q)
	{
		return Quat(
			a / q._qx,
			a / q._qy,
			a / q._qz,
			a / q._qw
		);
	}

	void Quat::Print(const char* pName) const
	{
		printf("Quat : %s  \n", pName);
		printf("(%f , %f, %f, %f \n", this->_qx, this->_qy, this->_qz, this->_qw);
	}

	Quat Quat::FromAxisAngle(const Vec3& axis, const float angle)
	{
		return Quat(Axis::AxisAngle, axis, angle);
	}

	void Quat::SetForward(const Vec3& forward, const Vec3* up)
	{
		Vec3 f = forward.getNorm();        // 正规化 forward
		Vec3 u;

		if (up)
			u = up->getNorm();             // 使用传入 up
		else
			u = this->Up().getNorm();      // 使用当前四元数的 up

		// 计算正交的 right 向量
		Vec3 r = u.cross(f).getNorm();

		// 重新计算正交 up 向量
		Vec3 orthoUp = f.cross(r);

		// 构造行优先旋转矩阵
		Rot R;

		R[m0] = r.x(); R[m1] = r.y(); R[m2] = r.z();
		R[m4] = orthoUp.x(); R[m5] = orthoUp.y(); R[m6] = orthoUp.z();
		R[m8] = f.x(); R[m9] = f.y(); R[m10] = f.z();

		// 更新四元数
		set(R);	
	}
}

//--- End of File ---
