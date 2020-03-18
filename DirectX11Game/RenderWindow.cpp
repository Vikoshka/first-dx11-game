#include "WindowContainer.h"

bool RenderWindow::Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    HRESULT hr;

    this->hInstance = hInstance;
    this->width = width;
    this->height = height;
    this->window_title = window_title;
    this->window_title_wide = StringConverter::StringToWide(window_title);
    this->window_class = window_class;
    this->window_class_wide = StringConverter::StringToWide(window_class);

    this->RegisterWindowClass();

    this->hWnd = CreateWindowExW(0,
        this->window_class_wide.c_str(),
        this->window_title_wide.c_str(),
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        0,
        0,
        this->width,
        this->height,
        NULL,
        NULL,
        this->hInstance,
        pWindowContainer);

    if (this->hWnd == NULL) 
    {
        ErrorLogger::Log(GetLastError(), "Failed to create Window " + this->window_title);
        return false;
    }

    ShowWindow(this->hWnd, SW_SHOW);
    SetForegroundWindow(this->hWnd);
    SetFocus(this->hWnd);

    return true;
}

LRESULT CALLBACK HandleMsgRedirect(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CLOSE:
    {
        DestroyWindow(hWnd);
        return 0;
    }
    default:
    {
        WindowContainer* const pWindow = reinterpret_cast<WindowContainer*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
    }
    }
}

LRESULT CALLBACK HandleMessageSetup(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_NCCREATE:
    {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        WindowContainer* pWindow = reinterpret_cast<WindowContainer*>(pCreate->lpCreateParams);
        if (pWindow == nullptr)
        {
            ErrorLogger::Log("Critical error: Pointer to window in null during WM_NCCREATE.");
            exit(-1);
        }
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWindow));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(HandleMsgRedirect));
        return pWindow->WindowProc(hWnd, uMsg, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
}

void RenderWindow::RegisterWindowClass()
{
    WNDCLASSEX wc;
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = HandleMessageSetup;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = this->hInstance;
    wc.hIcon = NULL;
    wc.hIconSm = NULL;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = this->window_class_wide.c_str();
    wc.cbSize = sizeof(WNDCLASSEX);
    RegisterClassEx(&wc);
}


bool RenderWindow::ProcessMessages()
{
    MSG msg;
    ZeroMemory(&msg, sizeof(MSG));

    if (PeekMessage(&msg,
        this->hWnd,
        0,
        0,
        PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if (msg.message == WM_NULL)
    {
        if (!IsWindow(this->hWnd))
        {
            this->hWnd = NULL;
            UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
            return false;
        }
    }

    return true;
}

RenderWindow::~RenderWindow()
{
    if (this->hWnd != NULL)
    {
        UnregisterClass(this->window_class_wide.c_str(), this->hInstance);
        DestroyWindow(hWnd);
    }
}

