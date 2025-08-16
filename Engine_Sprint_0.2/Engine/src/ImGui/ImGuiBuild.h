#ifndef IMGUI_BUILD_H
#define IMGUI_BUILD_H

#include "MathEngine.h"
#include "imGui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace Azul
{
	class ImGuiBuild
	{
	private:

	public:
		ImGuiBuild() = default;
		void DrawViewPort(bool& viewPortOpen);
		void DrawSettingPort(bool& Setting);
		void DrawWindow(bool & open);
		void ShowExampleAppDockSpace(bool* p_open);

		Vec2 mViewPortSize;
	};
}


#endif // !IMGUI_BUILD_H