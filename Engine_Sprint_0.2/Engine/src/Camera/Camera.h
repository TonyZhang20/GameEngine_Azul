//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------

#ifndef CAMERA_H
#define CAMERA_H

#include "MathEngine.h"

namespace Azul
{
	class Camera 
	{
	public:
		enum class Name
		{
			Default,
			CAMERA_A,
			CAMERA_B,
			CAMERA_C,
			CAMERA_D,

			CAMERA_E,
			CAMERA_F,
			CAMERA_G,
			CAMERA_L,
			MAIN_CAMERA,
			NULL_CAMERA,
			NOT_INITIALIZED,
		};

	public:
		// Default constructor
		Camera();
		Camera(const Camera&) = default;
		Camera& operator = (const Camera&) = default;

		virtual ~Camera() = default;

		void SetAspectRatio(float ratio);

		// Setup on single camera
		void setOrthographic(const float width, const float height, const float nearDist, const float farDist);
		void setPerspective(const float FieldOfView_Degs, const float AspectRatio, const float NearDist, const float FarDist);
		void setOrientAndPosition(const Vec3& Up_vect, const Vec3& inLookAt_pt, const Vec3& pos_pt);

		// helper functions
		void GetHelper(Vec3& up, Vec3& tar, Vec3& pos, Vec3& upNorm, Vec3& forwardNorm, Vec3& rightNorm);
		void SetHelper(Vec3& up, Vec3& tar, Vec3& pos);

		// update camera system
		virtual void updateCamera(void);

		// Get the matrices for rendering
		Mat4 getViewMatrix(Quaternion& q, Vec3& vPos);
		Mat4& getProjMatrix();


		// Why no SETS for Pos, Dir, Up, LookAt and Right?
		//   They have to be adjusted _together_ in setOrientAndPosition()
		void SetFov(const float fov);
		void SetName(Name _name);
		char* GetName();

		float& GetNear();
		float& GetFar();
		float& GetFov();


	public:
		Name name;

	private:  // methods should never be public
		void privUpdateProjectionMatrix(void);


	private:  // data  (Keep it private)
		// Projection Matrix
		Mat4	projMatrix;

		// Define the frustum inputs
		float	nearDist;
		float	farDist;
		float	fovy;  // aka view angle along y axis
		float	aspectRatio;
	};

}


#endif

// --- End of File ---