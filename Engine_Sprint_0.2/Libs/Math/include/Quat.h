//-----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------- 

#ifndef ENGINE_MATH_QUAT_H
#define ENGINE_MATH_QUAT_H

#include "Math_DLLInterface.h"

#include "Mat4.h"
#include "Enum.h"
//------------------------------------------------------------------------
// 
//  This class contains functions and data structure for a 4-dimensional
//  Quats.  The Quats are internally stored interms of the
//  imaginary components (x,y,z) and the real components (real).
// 
//  In the Math library, Quats can be thought of as "rotational"
//  matrices, everything that you can represent with rotational matrices
//  you can use with Quats.  Such as Quat concatenation,
//  inverses, transpose, normalize, multiplying with matrices, and with
//  vectors.
// 
//------------------------------------------------------------------------
namespace Azul
{
	class Quat final : public Align16
	{
	public:

		// Do your magic here

		// Constructors

		MATH_LIBRARY_API Quat(void);
		MATH_LIBRARY_API Quat(const Quat &qIn);
		MATH_LIBRARY_API Quat(const Vec3 &vect, const float real);
		MATH_LIBRARY_API Quat(const float vx, const float vy, const float vz, const float real);

		MATH_LIBRARY_API explicit Quat(const enum Identity_enum);
		MATH_LIBRARY_API explicit Quat(const Rot &);
		MATH_LIBRARY_API Quat(const Rot1 type, const float angle);
		MATH_LIBRARY_API Quat(const Rot3 type, const float angle_x, const float angle_y, const float angle_z);
		MATH_LIBRARY_API Quat(const Axis type, const Vec3 &vAxis, const float angle_radians);
		MATH_LIBRARY_API Quat(const Orient, const Vec3 &dof, const Vec3 &up);
		MATH_LIBRARY_API ~Quat(void);

		// Set
		MATH_LIBRARY_API void set(const enum Identity_enum);
		MATH_LIBRARY_API void set(const Quat&);
		MATH_LIBRARY_API void set(const Rot &);
		MATH_LIBRARY_API void set(const Rot1 type, const float angle);
		MATH_LIBRARY_API void set(const Rot3 type, const float angle_x, const float angle_y, const float angle_z);

		MATH_LIBRARY_API void set(const Axis type, const Vec3 &vAxis, const float angle_radians);
		MATH_LIBRARY_API void set(const Orient, const Vec3 &dof, const Vec3 &up);
		MATH_LIBRARY_API void set(const float vx, const float vy, const float vz, const float real);

		MATH_LIBRARY_API float &operator[](const x_enum value);
		MATH_LIBRARY_API float &operator[](const y_enum value);
		MATH_LIBRARY_API float &operator[](const z_enum value);
		MATH_LIBRARY_API float &operator[](const w_enum value);

		MATH_LIBRARY_API void qx(const float);
		MATH_LIBRARY_API void qy(const float);
		MATH_LIBRARY_API void qz(const float);

		// these two are the same, just an alias
		MATH_LIBRARY_API void qw(const float);
		MATH_LIBRARY_API void real(const float);

		MATH_LIBRARY_API void setVec3(const Vec3 &vect);
		MATH_LIBRARY_API void set(const Vec3 &vect, const float real);

		// Get

		MATH_LIBRARY_API float operator[](const x_enum value) const;
		MATH_LIBRARY_API float operator[](const y_enum value) const;
		MATH_LIBRARY_API float operator[](const z_enum value) const;
		MATH_LIBRARY_API float operator[](const w_enum value) const;

		MATH_LIBRARY_API float qx()const;
		MATH_LIBRARY_API float qy()const;
		MATH_LIBRARY_API float qz()const;

		// these two are the same, just an alias
		MATH_LIBRARY_API float qw()const;
		MATH_LIBRARY_API float real()const;

		MATH_LIBRARY_API float getAngle(void) const;
		MATH_LIBRARY_API void getVec3(Vec3 &vOut) const;
		MATH_LIBRARY_API void getAxis(Vec3 &vOut) const;

		// General member functions

		// dot product of two Quats (4D)
		MATH_LIBRARY_API float dot(const Quat &qin) const;

		// Magnitude
		MATH_LIBRARY_API float mag(void) const;

		// magnitude Squared (Remember for Unit Quats, magSquared == 1.0f );
		MATH_LIBRARY_API float magSquared(void) const;
		MATH_LIBRARY_API float invMag(void) const;

		MATH_LIBRARY_API Vec3 Forward(void) const;
		MATH_LIBRARY_API Vec3 Right(void) const;
		MATH_LIBRARY_API Vec3 Up(void) const;

		// Conjugate

		// Conjugate Quat in place
		MATH_LIBRARY_API Quat &conj(void);

		// Return conjuate Quat
		MATH_LIBRARY_API Quat getConj(void) const;

		// Inverse

		// Inverse Quat in place
		MATH_LIBRARY_API Quat &inv(void);

		// Return inverse Quat
		MATH_LIBRARY_API Quat getInv(void) const;

		// Normalize

		// Normalize Quat in place
		MATH_LIBRARY_API Quat &norm(void);

		// Return Normalize Quat
		MATH_LIBRARY_API Quat getNorm(void) const;

		// Linear Operator

		// Linear Operator vout = Quat * Vector * Quat(conjugate)
		// vOut = Quat * vIn * Quat.conj()
		MATH_LIBRARY_API void Lqvqc(const Vec3 &vIn, Vec3 &vOut) const;

		// Linear Operator  vOut = Quat(conjugate) * Vector * Quat
		// vOut = Quat.conj() * vIn * Quat		
		MATH_LIBRARY_API void Lqcvq(const Vec3 &vIn, Vec3 &vOut) const;

		// Test functions

		MATH_LIBRARY_API bool isEqual(const Quat &qin, const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isEquivalent(const Quat &qin, const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isNegEqual(const Quat &qin, const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isConjugateEqual(const Quat &qin, const float epsilon = MATH_TOLERANCE) const;

		MATH_LIBRARY_API bool isIdentity(const float epsilon = MATH_TOLERANCE) const;
		MATH_LIBRARY_API bool isNormalized(const float epsilon = MATH_TOLERANCE) const;

		// Overload the = operator  

		MATH_LIBRARY_API Quat &operator=(const Quat &q);
		MATH_LIBRARY_API Quat &operator=(const Rot &m);

		// Overload the + operator

		MATH_LIBRARY_API Quat operator+(void) const;
		MATH_LIBRARY_API Quat operator+(const Quat &q) const;
		MATH_LIBRARY_API Quat &operator+=(const Quat &q);
		MATH_LIBRARY_API Quat operator+(const float a) const;
		MATH_LIBRARY_API Quat &operator+=(const float a);
		MATH_LIBRARY_API friend Quat operator+(const float a, const Quat &q);

		// Overload the - operator

		MATH_LIBRARY_API Quat operator-(void) const;
		MATH_LIBRARY_API Quat operator-(const Quat &q) const;
		MATH_LIBRARY_API Quat &operator-=(const Quat &q);
		MATH_LIBRARY_API Quat operator-(const float a) const;
		MATH_LIBRARY_API Quat &operator-=(const float a);
		MATH_LIBRARY_API friend Quat operator-(const float a, const Quat &q);

		// Overload the * operator

		MATH_LIBRARY_API Quat operator*(const float a) const;
		MATH_LIBRARY_API Quat &operator*=(const float a);
		MATH_LIBRARY_API friend Quat operator*(const float a, const Quat &q);

		MATH_LIBRARY_API Quat &operator*=(const Mat4 &m) = delete;
		MATH_LIBRARY_API Mat4 operator*(const Mat4 &m) const;

		MATH_LIBRARY_API Quat operator*(const Quat &q) const;
		MATH_LIBRARY_API Quat &operator*=(const Quat &q);

		MATH_LIBRARY_API Mat4 &operator*=(const Scale &m) = delete;
		MATH_LIBRARY_API Mat4 operator*(const Scale &m) const;

		MATH_LIBRARY_API Quat &operator*=(const Rot &m);
		MATH_LIBRARY_API Rot operator*(const Rot &m) const;

		MATH_LIBRARY_API Mat4 &operator*=(const Trans &m) = delete;
		MATH_LIBRARY_API Mat4 operator*(const Trans &m) const;

		// Overload the / operator

		MATH_LIBRARY_API Quat operator/(const Quat &q) const;
		MATH_LIBRARY_API Quat &operator/=(const Quat &q);
		MATH_LIBRARY_API Quat operator/(const float a) const;
		MATH_LIBRARY_API Quat &operator/=(const float a);
		MATH_LIBRARY_API friend Quat operator/(const float a, const Quat &q);

		// For printing
		MATH_LIBRARY_API void Print(const char *pName) const;

		MATH_LIBRARY_API static Quat FromAxisAngle(const Vec3& axis, const float angle);
		MATH_LIBRARY_API void SetForward(const Vec3& forward, const Vec3* up);
		MATH_LIBRARY_API void SetFromRotationMatrix(const Mat3& rotMat);
		
	private:
		friend Vec4;
		friend Mat4;
		friend Rot;
		friend Scale;
		friend Trans;
		friend Mat4;

		// anonymous union
		union
		{
			__m128	_mq;

			// anonymous struct
			struct
			{
				float _qx;
				float _qy;
				float _qz;
				union
				{
					float _qw;
					float _real;
				};
			};

			struct
			{
				Vec3  _qV3;
			};

			struct
			{
				Vec4 _qV4;
			};
		};

	};
}

#endif 

//--- End of File ---
