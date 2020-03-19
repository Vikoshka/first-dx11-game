#include "WindowContainer.h"

WindowContainer::WindowContainer()
{
    static bool raw_input_initialized = false;

    if (raw_input_initialized == false)
    {
        RAWINPUTDEVICE rid;

        rid.usUsagePage = 0x01;
        rid.usUsage = 0x02;
        rid.dwFlags = 0;
        rid.hwndTarget = NULL;

        if (RegisterRawInputDevices(&rid, 1, sizeof(rid)) == FALSE)
        {
            ErrorLogger::Log(GetLastError(), "FAILED to register input device.");
            exit(-1);
        }
        raw_input_initialized = true;
    }
}

LRESULT WindowContainer::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    //Keyboard messages
    case WM_KEYDOWN:
    {
        UCHAR keycode = static_cast<UCHAR>(wParam);
        if (keyboard.IsKeysAutoRepeat())
        {
            keyboard.OnKeyPressed(keycode);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                keyboard.OnKeyPressed(keycode);
            }
        }
        return 0;
    }
    case WM_KEYUP:
    {
        UCHAR keycode = static_cast<UCHAR>(wParam);
        keyboard.OnKeyReleased(keycode);
        return 0;
    }
    case WM_CHAR:
    {
        UCHAR ch = static_cast<UCHAR>(wParam);
        if (keyboard.IsCharsAutoRepeat())
        {
            keyboard.OnChar(ch);
        }
        else
        {
            const bool wasPressed = lParam & 0x40000000;
            if (!wasPressed)
            {
                keyboard.OnChar(ch);
            }
        }
        return 0;
    }
    //Mouse Messages
    case WM_MOUSEMOVE:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnMouseMove(x, y);
        return 0;
    }
    case WM_LBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnLeftPressed(x, y);
    }
    case WM_LBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnLeftReleased(x, y);
    }
    case WM_RBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnRightPressed(x, y);
    }
    case WM_RBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnRightReleased(x, y);
    }
    case WM_MBUTTONDOWN:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnMiddlePressed(x, y);
    }
    case WM_MBUTTONUP:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        mouse.OnMiddleReleased(x, y);
    }
    case WM_MOUSEWHEEL:
    {
        int x = LOWORD(lParam);
        int y = HIWORD(lParam);
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
        {
            mouse.OnWheelUp(x, y);
        }
        if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
        {
            mouse.OnWheelDown(x, y);
        }
    }
    case WM_INPUT:
    {
        //Check for data exist
        UINT dataSize;
        GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, NULL, &dataSize, sizeof(RAWINPUTHEADER));

        //if data is exist we getting it's content
        if (dataSize > 0)
        {
            std::unique_ptr<BYTE[]> rawdata = std::make_unique<BYTE[]>(dataSize);
            if (GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT, rawdata.get(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize)
            {

                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(rawdata.get());
                if (raw->header.dwType == RIM_TYPEMOUSE)
                {
                    mouse.OnMouseMoveRaw(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
                }
            }
        }

        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}
