#pragma once
#include "../Common/ErrorLogger.h"

class WindowContainer;

class RenderWindow
{
public:
    bool Initialize(WindowContainer* pWindowContainer, HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height);
    bool ProcessMessages();
    HWND GetHWND();
    ~RenderWindow();
private:
    void RegisterWindowClass();
    HWND hWnd = NULL;
    HINSTANCE hInstance = NULL;
    std::string window_title = "";
    std::wstring window_title_wide = L"";
    std::string window_class = "";
    std::wstring window_class_wide = L"";
    int width = 0;
    int height = 0;
};

