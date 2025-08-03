#include "Input.h"

namespace Azul
{
	void Input::Update()
	{
        
        // Save previous key/mouse states
        memcpy(Get().prevKeys, Get().currKeys, sizeof(currKeys));
        memcpy(Get().prevMouse, Get().currMouse, sizeof(Get().currMouse));

        // Update key state
        for (int i = 0; i < 256; ++i)
        {
            Get().currKeys[i] = (GetAsyncKeyState(i) & 0x8000) != 0;
        }

        // Update mouse buttons
        Get().currMouse[0] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
        Get().currMouse[1] = (GetAsyncKeyState(VK_RBUTTON) & 0x8000) != 0;
        Get().currMouse[2] = (GetAsyncKeyState(VK_MBUTTON) & 0x8000) != 0;

        // Update mouse position
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(GetForegroundWindow(), &pt);
        Get().mousePos = Vec2((float)pt.x, (float)pt.y);
        
	}
}