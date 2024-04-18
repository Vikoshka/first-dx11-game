#pragma once
#include "Graphics/RenderWindow.h"
#include "InputOutput/KeyboardClass.h"
#include "InputOutput/MouseClass.h"
#include <memory>

class WindowContainer
{
public:
    WindowContainer();
    LRESULT WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    RenderWindow render_window;
    KeyboardClass keyboard;
    MouseClass mouse;
};

