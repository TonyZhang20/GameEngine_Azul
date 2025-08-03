#ifndef INPUT_H
#define INPUT_H

#include <Windows.h>
#include "MathEngine.h"

namespace Azul
{
    enum class KeyCode : unsigned short
    {
        None = 0x00,

        // 常规键盘按键
        Backspace = VK_BACK,
        Tab = VK_TAB,
        Enter = VK_RETURN,
        Shift = VK_SHIFT,
        Control = VK_CONTROL,
        Alt = VK_MENU,
        Escape = VK_ESCAPE,
        Space = VK_SPACE,
        Left = VK_LEFT,
        Right = VK_RIGHT,
        Up = VK_UP,
        Down = VK_DOWN,
        A = 'A', B = 'B', C = 'C', D = 'D', E = 'E', F = 'F',
        G = 'G', H = 'H', I = 'I', J = 'J', K = 'K', L = 'L',
        M = 'M', N = 'N', O = 'O', P = 'P', Q = 'Q', R = 'R',
        S = 'S', T = 'T', U = 'U', V = 'V', W = 'W', X = 'X',
        Y = 'Y', Z = 'Z',
        Num0 = '0', Num1 = '1', Num2 = '2', Num3 = '3',
        Num4 = '4', Num5 = '5', Num6 = '6', Num7 = '7',
        Num8 = '8', Num9 = '9',

        // 鼠标按键映射
        MouseLeft = VK_LBUTTON,
        MouseRight = VK_RBUTTON,
        MouseMiddle = VK_MBUTTON
    };

    class Input
    {
    public:
        static Input& Get()
        {
            static Input instance;
            return instance;
        }

        static void Update();


        // Key
        static bool GetKey(KeyCode key) { return Get().currKeys[static_cast<int>(key)]; }
        static bool GetKeyDown(KeyCode key)
        {
            int i = static_cast<int>(key);
            return Get().currKeys[i] && !Get().prevKeys[i];
        }
        static bool GetKeyUp(KeyCode key)
        {
            int i = static_cast<int>(key);
            return !Get().currKeys[i] && Get().prevKeys[i];
        }

        // Mouse
        static bool GetMouse(KeyCode button)
        {
            int idx = MouseIndex(button);
            return idx >= 0 ? Get().currMouse[idx] : false;
        }

        static bool GetMouseDown(KeyCode button)
        {
            int idx = MouseIndex(button);
            return idx >= 0 ? Get().currMouse[idx] && !Get().prevMouse[idx] : false;
        }

        static bool GetMouseUp(KeyCode button)
        {
            int idx = MouseIndex(button);
            return idx >= 0 ? !Get().currMouse[idx] && Get().prevMouse[idx] : false;
        }

        static Vec2 GetMousePos()
        {
            return Get().mousePos;
        }

    private:
        Input() = default;

        static int MouseIndex(KeyCode code)
        {
            switch (code)
            {
            case KeyCode::MouseLeft: return 0;
            case KeyCode::MouseRight: return 1;
            case KeyCode::MouseMiddle: return 2;
            default: return -1;
            }
        }

        bool currKeys[256]{};
        bool prevKeys[256]{};

        bool currMouse[3]{};
        bool prevMouse[3]{};

        Vec2 mousePos{ 0.0f, 0.0f };
    };
}

#endif
