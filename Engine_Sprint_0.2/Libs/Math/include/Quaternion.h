#include <xmmintrin.h>
#include <smmintrin.h> 

#include "Constants.h"
#include "MathExportAPI.h"

namespace Azul
{
	class Vec4;
	class Vec3;
	class Mat4;


	class Quaternion final : public Align16
	{

	public:
		// Big 4
		MATHLIBRARY_API Quaternion();
		MATHLIBRARY_API Quaternion(const Quaternion& q);
		MATHLIBRARY_API Quaternion& operator=(const Quaternion& q);
		MATHLIBRARY_API ~Quaternion();

		// Big 6
		MATHLIBRARY_API Quaternion(Quaternion&& q) = default;
		MATHLIBRARY_API Quaternion& operator=(Quaternion&& q) = default;

		// Constructors
		MATHLIBRARY_API Quaternion(const float x, const float y, const float z, const float w);
		MATHLIBRARY_API Quaternion(const Vec3& axis, const float angleRad); // axis-angle
		MATHLIBRARY_API Quaternion(const Vec4& v); // construct from Vec4

		// Accessors
		MATHLIBRARY_API void x(const float v);
		MATHLIBRARY_API void y(const float v);
		MATHLIBRARY_API void z(const float v);
		MATHLIBRARY_API void w(const float v);

		MATHLIBRARY_API float x() const;
		MATHLIBRARY_API float y() const;
		MATHLIBRARY_API float z() const;
		MATHLIBRARY_API float w() const;

		// Operators
		MATHLIBRARY_API Quaternion operator+(const Quaternion& q) const;
		MATHLIBRARY_API Quaternion& operator+=(const Quaternion& q);

		MATHLIBRARY_API Quaternion operator-(const Quaternion& q) const;
		MATHLIBRARY_API Quaternion& operator-=(const Quaternion& q);

		MATHLIBRARY_API Quaternion operator*(const Quaternion& q) const;  // quaternion multiplication
		MATHLIBRARY_API Quaternion& operator*=(const Quaternion& q);

		MATHLIBRARY_API Quaternion operator*(const float s) const;
		MATHLIBRARY_API Quaternion& operator*=(const float s);

		// Functions
		MATHLIBRARY_API void set(const float x, const float y, const float z, const float w);
		MATHLIBRARY_API void set(const Quaternion& q);

		MATHLIBRARY_API Quaternion& norm();        // normalize
		MATHLIBRARY_API Quaternion getNorm() const;

		MATHLIBRARY_API Quaternion conjugate() const;
		MATHLIBRARY_API Quaternion inverse() const;

		MATHLIBRARY_API float dot(const Quaternion& q) const;

		MATHLIBRARY_API static Quaternion Identity();
		MATHLIBRARY_API static Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t);

		MATHLIBRARY_API void Print(const char* pName) const;

		MATHLIBRARY_API Vec3 Up() const;
		MATHLIBRARY_API Vec3 Right() const;
		MATHLIBRARY_API Vec3 Forward() const;

		MATHLIBRARY_API Mat4 ToMatrix() const;

		MATHLIBRARY_API static Vec3 GetUp(const Quaternion& q);
		MATHLIBRARY_API static Vec3 GetRight(const Quaternion& q);
		MATHLIBRARY_API static Vec3 GetForward(const Quaternion& q);

	private:
		friend Vec4;
		friend Vec3;
		friend Mat4;

		union
		{
			__m128 _mq;
			struct
			{
				float _qx;
				float _qy;
				float _qz;
				float _qw;
			};
		};
	};
}
