#include "Graphics.h"

bool Graphics::Initialize(HWND hWnd, int width, int height)
{
    if (!InitializeDirectX(hWnd, x, y))
        return false;

    return true;
}

bool Graphics::InitializeDirectX(HWND hWnd, int width, int height)
{


    HRESULT hr = D3D11CreateDeviceAndSwapChain();


    return true;
}
