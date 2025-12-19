//--------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//--------------------------------------------------------------
#include "MathEngine.h"
#include "Camera.h"
#include "Input.h"
#include "StringThis.h"
#include "Components.h"

namespace Azul
{
	const float moveSpeed = 15.f;
	const float mouseSensitivity = 0.2f;//鼠标移动速度
	Camera::Camera()
		: aspectRatio(0), farDist(0), fovy(0), nearDist(0), projMatrix()
	{
	}

	void Camera::GetHelper(Vec3& up, Vec3& tar, Vec3& pos,
		Vec3& upNorm, Vec3& forwardNorm, Vec3& pRightNorm)
	{

	}

	void Camera::SetHelper(Vec3& up_pt, Vec3& tar_pt, Vec3& pos_pt)
	{
		Vec3 upVect = up_pt - pos_pt;
		this->setOrientAndPosition(upVect, tar_pt, pos_pt);
	}


	void Camera::SetAspectRatio(float ratio)
	{
		this->aspectRatio = ratio;
	}

	void Camera::setOrthographic(const float width, const float height, const float nearZ, const float farZ)
	{
		this->aspectRatio = width / height;
		this->nearDist = nearZ;
		this->farDist = farZ;

		// Orthographi（left hand）
		this->projMatrix[m0] = 2.0f / width;
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = 2.0f / height;
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = 1.0f / (farZ - nearZ);  // or 2.0 / (farZ - nearZ)
		this->projMatrix[m11] = 0.0f;

		this->projMatrix[m12] = 0.0f;
		this->projMatrix[m13] = 0.0f;
		this->projMatrix[m14] = -nearZ / (farZ - nearZ);  //  -nearZ - farZ 偏移
		this->projMatrix[m15] = 1.0f;

		// DX -> NDC from [-1,1] -> [0,1]
		Trans B(0.0f, 0.0f, 1.0f);
		Scale S(1.0f, 1.0f, 0.5f);
		projMatrix = projMatrix * B * S;
	}

	void Camera::setPerspective(const float Fovy, const float Aspect, const float NearDist, const float FarDist)
	{
		this->aspectRatio = Aspect;
		this->fovy = MATH_PI_180 * Fovy;
		this->nearDist = NearDist;
		this->farDist = FarDist;
	};


	void Camera::setOrientAndPosition(const Vec3& inUp, const Vec3& inLookAt, const Vec3& inPos)
	{

	};

	void Camera::SetFov(const float fov)
	{
		this->fovy = fov;
	}


	void Camera::SetName(Name _name)
	{
		this->name = _name;
	}

	char* Camera::GetName()
	{
		// todo - Hack understand why is this needed for print and fix...
		static char pTmp[128];
		strcpy_s(pTmp, 128, StringMe(this->name));
		return pTmp;
	}

	float& Camera::GetNear()
	{
		return this->nearDist;
	}

	float& Camera::GetFar()
	{
		return this->farDist;
	}

	float& Camera::GetFov()
	{
		return this->fovy;
	}

	// The projection matrix 
	void Camera::privUpdateProjectionMatrix(void)
	{
		float d = 1 / tanf(fovy / 2);

		this->projMatrix[m0] = d / aspectRatio;
		this->projMatrix[m1] = 0.0f;
		this->projMatrix[m2] = 0.0f;
		this->projMatrix[m3] = 0.0f;

		this->projMatrix[m4] = 0.0f;
		this->projMatrix[m5] = d;
		this->projMatrix[m6] = 0.0f;
		this->projMatrix[m7] = 0.0f;

		this->projMatrix[m8] = 0.0f;
		this->projMatrix[m9] = 0.0f;
		this->projMatrix[m10] = -(this->farDist + this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[m11] = -1.0f;

		this->projMatrix[m12] = 0.0f;
		this->projMatrix[m13] = 0.0f;
		this->projMatrix[m14] = (-2.0f * this->farDist * this->nearDist) / (this->farDist - this->nearDist);
		this->projMatrix[m15] = 0.0f;

		// Converting from OpenGL-style NDC of [-1,1] to DX's [0,1].  See note: https://anteru.net/blog/2011/12/27/1830/
		// (Note: NDCConvert should be precomputed once and stored for reuse)
		Trans B(0.0f, 0.0f, 1.0f);
		Scale S(1.0f, 1.0f, 0.5f);

		projMatrix = projMatrix * B * S;
	}
	
	Vec2f prevMousePos;

	void Camera::privUpdateCameraMove(float deltaTime)
	{
		auto& trans = this->pCamEntity->GetComponent<TransformComponent>();

		if (Input::GetKey(KeyCode::D))
		{
			MoveRight(deltaTime);
		}

		if (Input::GetKey(KeyCode::A))
		{
			MoveRight(-deltaTime);
		}

		if (Input::GetKey(KeyCode::S))
		{
			MoveForward(deltaTime);
		}

		if(Input::GetKey(KeyCode::W))
		{
			MoveForward(-deltaTime);
		}

		MouseInput(deltaTime);

		//...
	}
	

	// Update everything (make sure it's consistent)
	void Camera::updateCamera(float deltaTime)
	{
		// update the projection matrix
		this->privUpdateProjectionMatrix();

		// update the view matrix
		this->privUpdateCameraMove(deltaTime);
	}

	Mat4& Camera::getProjMatrix(void)
	{
		return this->projMatrix;
	}

	Mat4 Camera::getViewMatrix(Quat& q, Vec3& vPos)
	{
		auto right = q.Right();
		auto up = q.Up();
		auto fwd = q.Forward();

		Mat4 viewMatrix{
			Vec4{right[x],  up[x],  fwd[x],  0.0f},
			Vec4{right[y],  up[y],  fwd[y],  0.0f},
			Vec4{right[z],  up[z],  fwd[z],  0.0f},
			Vec4{-vPos.dot(right), -vPos.dot(up), -vPos.dot(fwd), 1.0f}
		};


		return viewMatrix;
	}


	void Camera::MoveForward(float amount)
	{
		if (!pCamEntity) return;

		auto& trans = this->pCamEntity->GetComponent<TransformComponent>();
		Vec3 forward = trans.Forward();
		trans.position += forward * amount * moveSpeed;
	}

	void Camera::MoveRight(float amount)
	{
		if (!pCamEntity) return;

		auto& trans = this->pCamEntity->GetComponent<TransformComponent>();
		Vec3 right = trans.Right();
		trans.position += right * amount * moveSpeed;
	}

	void Camera::MouseInput(float /*deltaTime*/)
	{
		if (!pCamEntity) return;
		auto& trans = pCamEntity->GetComponent<TransformComponent>();

		static float yaw = 0.0f;      // 弧度
		static float pitch = 0.0f;    // 弧度
		static Vec3  lastRight(1.0f, 0.0f, 0.0f);
		static bool  ignoreNextDelta = false;

		const Vec3 worldUp(0.0f, 1.0f, 0.0f);

		if (!Input::GetKey(KeyCode::MouseLeft))
			return;

		if (Input::GetKeyDown(KeyCode::MouseLeft))
		{
			prevMousePos = Input::GetMousePos();
			ignoreNextDelta = true;   // 应对锁鼠标/warp

			// 只用 Forward 同步 yaw/pitch（你已确认 Identity forward = (0,0,1)）
			Vec3 f = trans.rotation.Forward();
			f.norm();

			yaw = atan2f(f.x(), f.z());   // Forward = +Z 的情况
			float fy = f.y();
			if (fy > 1.0f) fy = 1.0f;
			if (fy < -1.0f) fy = -1.0f;
			pitch = asinf(fy);

			// ★不要用 trans.rotation.Right()！用 worldUp×forward 得到稳定 right
			Vec3 r = worldUp.cross(f);
			if (r.len() > 0.0001f) { r.norm(); lastRight = r; }

			return; // 按下这一帧不改旋转
		}

		// KeyDown 后第一帧，通常发生 warp，直接同步并丢掉
		if (ignoreNextDelta)
		{
			prevMousePos = Input::GetMousePos();
			ignoreNextDelta = false;
			return;
		}

		Vec2f cur = Input::GetMousePos();
		float dx = cur.x - prevMousePos.x;
		float dy = cur.y - prevMousePos.y;
		prevMousePos = cur;

		// ★关键：如果你的系统在这一帧又 warp/锁定，dx/dy 会突然巨大 -> 丢掉这帧避免跳
		// 这个阈值按你的窗口/坐标尺度可调（如果是像素坐标，200~800 都常见）
		if (fabsf(dx) > 200.0f || fabsf(dy) > 200.0f)
		{
			// 认为是 warp，不更新角度
			return;
		}

		if (fabsf(dx) < 0.01f && fabsf(dy) < 0.01f)
			return;

		const float k = 0.0025f; // 像素->弧度
		yaw -= dx * mouseSensitivity * k;
		pitch += dy * mouseSensitivity * k;
		// 上下反就改成：pitch -= dy * mouseSensitivity * k;

		const float pitchLimit = 89.0f * MATH_PI_180;
		if (pitch > pitchLimit) pitch = pitchLimit;
		if (pitch < -pitchLimit) pitch = -pitchLimit;

		// 用 yaw/pitch 重建 forward（Forward = +Z）
		float cp = cosf(pitch);
		Vec3 fwd(
			sinf(yaw) * cp,
			sinf(pitch),
			cosf(yaw) * cp
		);
		fwd.norm();

		Vec3 right = worldUp.cross(fwd);
		if (right.len() < 0.0001f)
		{
			right = lastRight; // 接近竖直时保持上一次
		}
		else
		{
			right.norm();
		}

		// 连续性修正：避免 right 突然取反导致左右颠倒
		if (right.dot(lastRight) < 0.0f)
			right = right * -1.0f;

		Vec3 up = fwd.cross(right);
		up.norm();

		lastRight = right;

		// 写入 Rot（按你 getViewMatrix 的行布局）
		Rot R;
		R[m0] = right.x(); R[m1] = right.y(); R[m2] = right.z(); 
		R[m4] = up.x(); R[m5] = up.y(); R[m6] = up.z();
		R[m8] = fwd.x(); R[m9] = fwd.y(); R[m10] = fwd.z(); 

		Quat q(R);
		q.norm();
		trans.rotation = q;
	}


}



// --- End of File ---
