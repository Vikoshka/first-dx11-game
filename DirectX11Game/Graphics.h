#pragma once
#include "AdapterReader.h"

class Graphics
{
public:
    bool Initialize(HWND hWnd, int width, int height);

private:
    bool InitializeDirectX(HWND hWnd, int width, int height);

    ID3D11Device* device;
    ID3D11DeviceContext* deviceContext;
    IDXGISwapChain* swapChain;
    ID3D11RenderTargetView* renderTargetView;
};

