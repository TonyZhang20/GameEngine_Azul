//----------------------------------------------------------------------------
// Copyright 2025, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef RENDER_H
#define RENDER_H
namespace Azul
{
	class Camera;
	class Render
	{
	public:
		virtual void SetGpu(Camera* pCam) = 0;
		virtual void Draw() = 0;
		virtual void RestoreState() = 0;
		virtual void SetState() {};

	protected:
		Render() = default;
		Render(const Render&) = delete;
		Render& operator = (const Render&) = delete;
		virtual ~Render() = default;

	};
}


#endif

// --- End of File ---
