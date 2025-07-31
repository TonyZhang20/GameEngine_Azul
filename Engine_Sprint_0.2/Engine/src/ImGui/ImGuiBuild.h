#ifndef IMGUI_BUILD_H
#define IMGUI_BUILD_H

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
		void DrawWindow(bool & open);
		void ShowExampleAppDockSpace(bool* p_open);

	};
}


#endif // !IMGUI_BUILD_H