#pragma once

#pragma comment(lib, "DirectXTK")

#include "AdapterReader.h"

#include <dxgi1_2.h>
#include <d3dcommon.h>
#include "Shaders.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"

#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <WICTextureLoader.h>


class Graphics
{
public:
    bool Initialize(HWND hWnd, int width, int height);
    void RenderFrame();

private:
    bool InitializeDirectX(HWND hWnd);
    bool InitializeShaders();
    bool InitializeScene();
    /*
    * TODO: Realize multisaple quality settings
    * 
    bool ChangeMultiSampleQuality()
    */

    Microsoft::WRL::ComPtr<ID3D11Device> device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> deviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> swapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
    Microsoft::WRL::ComPtr<ID3D11Texture2D> depthStencilBuffer;
    Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState;

    VertexShader vertexShader;
    PixelShader pixelShader;
    ConstantBuffer<CB_VS_sprite2D> constantBuffer;

    VertexBuffer<Vertex> vertexBuffer;
    IndexBuffer indexBuffer;

    Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

    std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
    std::unique_ptr<DirectX::SpriteFont> spriteFont;

    Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerState;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> myTexture;

    DirectX::XMMATRIX worldMatrix;
    int wndWidth = 0;
    int wndHeight = 0;
};

