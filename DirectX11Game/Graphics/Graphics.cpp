#include "Graphics.h"

bool Graphics::Initialize(HWND hWnd, int width, int height)
{
    wndWidth = width;
    wndHeight = height;

    if (!InitializeDirectX(hWnd))
        return false;

    if (!InitializeShaders())
        return false;

    if (!InitializeScene())
        return false;

    return true;
}

void Graphics::RenderFrame()
{
    float bgcolor[] = { 0.3f, 0.3f, 0.3f, 1.0f };
    this->deviceContext->ClearRenderTargetView(renderTargetView.Get(), bgcolor);
    this->deviceContext->ClearDepthStencilView(this->depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    this->deviceContext->IASetInputLayout(this->vertexShader.GetInputLayout());
    this->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    this->deviceContext->RSSetState(this->rasterizerState.Get());
    this->deviceContext->OMSetDepthStencilState(this->depthStencilState.Get(), 0);
    this->deviceContext->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
    this->deviceContext->VSSetShader(vertexShader.GetShader(), NULL, 0);
    this->deviceContext->PSSetShader(pixelShader.GetShader(), NULL, 0);

    UINT offset = 0;

    //Update pic
    this->constantBuffer.data.origin.x = 0.0f;
    this->constantBuffer.data.origin.y = 0.0f;
    this->constantBuffer.data.translation.x = 0.0f;
    this->constantBuffer.data.translation.y = 0.0f;
    this->constantBuffer.data.scale.x = 0.4f;
    this->constantBuffer.data.scale.y = 0.4f;
    this->constantBuffer.data.rotation.a = -0.4f;
    if (!this->constantBuffer.ApplyChanges())
        return;

    this->deviceContext->VSSetConstantBuffers(0, 1, this->constantBuffer.GetAddressOf());

    this->deviceContext->PSSetShaderResources(0, 1, this->myTexture.GetAddressOf());
    this->deviceContext->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), vertexBuffer.StridePtr(), &offset);
    this->deviceContext->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    this->deviceContext->DrawIndexed(4, 0, 0);

    //Draw text
    spriteBatch->Begin();
    spriteFont->DrawString(spriteBatch.get(), L"Meow Meow", DirectX::XMFLOAT2(0, 0), DirectX::Colors::Wheat, 0.0f, DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f));
    spriteBatch->End();

    this->swapChain->Present(1, NULL);
}

bool Graphics::InitializeDirectX(HWND hWnd)
{
    DXGI_MODE_DESC swapChainBufferDesc;
    ZeroMemory(&swapChainBufferDesc, sizeof(swapChainBufferDesc));
    swapChainBufferDesc.Width = wndWidth;
    swapChainBufferDesc.Height = wndHeight;
    swapChainBufferDesc.RefreshRate.Numerator = 60;
    swapChainBufferDesc.RefreshRate.Denominator = 1;
    swapChainBufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainBufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainBufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SAMPLE_DESC swapChainSampleDesc;
    ZeroMemory(&swapChainSampleDesc, sizeof(swapChainSampleDesc));
    swapChainSampleDesc.Count = 1;
    swapChainSampleDesc.Quality = 0;

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.SampleDesc = swapChainSampleDesc;
    swapChainDesc.BufferDesc = swapChainBufferDesc;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    

    auto hr = D3D11CreateDeviceAndSwapChain(
        NULL,
        D3D_DRIVER_TYPE_HARDWARE,
        NULL,
        NULL,
        NULL,
        0,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        swapChain.GetAddressOf(),
        device.GetAddressOf(),
        NULL,
        deviceContext.GetAddressOf());

    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Could not create Device and Swap chain");
        return false;
    }

    this->deviceContext->ClearState();
    
    ID3D11Texture2D* backBuffer;
    hr = this->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "GetBuffer failed.");
        return false;
    }

    hr = this->device->CreateRenderTargetView(backBuffer, NULL, this->renderTargetView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Could not create render target view.");
        return false;
    }
    
    D3D11_TEXTURE2D_DESC depthStencilBufferDesc;
    depthStencilBufferDesc.Width = wndWidth;
    depthStencilBufferDesc.Height = wndHeight;
    depthStencilBufferDesc.MipLevels = 1;
    depthStencilBufferDesc.ArraySize = 1;
    depthStencilBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilBufferDesc.SampleDesc.Count = 1;
    depthStencilBufferDesc.SampleDesc.Quality = 0;
    depthStencilBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthStencilBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthStencilBufferDesc.CPUAccessFlags = 0;
    depthStencilBufferDesc.MiscFlags = 0;

    hr = this->device->CreateTexture2D(&depthStencilBufferDesc, NULL, this->depthStencilBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil buffer.");
        return false;
    }
    
    hr = device->CreateDepthStencilView(this->depthStencilBuffer.Get(), NULL, this->depthStencilView.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil view.");
        return false;
    }

    this->deviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), this->depthStencilView.Get());

    D3D11_VIEWPORT viewport;
    ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = wndWidth;
    viewport.Height = wndHeight;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    this->deviceContext->RSSetViewports(1, &viewport);

    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));

    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    hr = this->device->CreateRasterizerState(&rasterizerDesc, this->rasterizerState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create rasterizer state.");
        return false;
    }

    //Create depth stencil state
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));

    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK::D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_FUNC::D3D11_COMPARISON_LESS_EQUAL;

    hr = this->device->CreateDepthStencilState(&depthStencilDesc, this->depthStencilState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create depth stencil state.");
        return false;
    }

    spriteBatch = std::make_unique<DirectX::SpriteBatch>(this->deviceContext.Get());
    spriteFont = std::make_unique<DirectX::SpriteFont>(this->device.Get(), L"Res\\Fonts\\klinik_16.spritefont");

    //Sampler desc for sampler state
    D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(D3D11_SAMPLER_DESC));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = this->device->CreateSamplerState(&sampDesc, this->samplerState.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create sampler state.");
        return false;
    }

    return true;
}

bool Graphics::InitializeShaders()
{
    


    std::wstring shaders_folder = L"";
    
    if (IsDebuggerPresent() == TRUE)
    {
#ifdef _DEBUG
    #ifdef _WIN64
        shaders_folder = L"..\\x64\\Debug\\";
    #else
        shaders_folder = L"..\\Debug\\";
    #endif
#else
    #ifdef _WIN64
        shaders_folder = L"..\\x64\\Release\\";
    #else
        shaders_folder = L"..\\Release\\";
    #endif
#endif

    }
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    if(!vertexShader.Initialize(this->device, shaders_folder + L"vertexshader.cso", layout, ARRAYSIZE(layout)))
    {
        return false;
    }

    if (!pixelShader.Initialize(this->device, shaders_folder + L"pixelshader.cso"))
    {
        return false;
    }

    return true;
}

bool Graphics::InitializeScene()
{
    worldMatrix = DirectX::XMMatrixOrthographicRH(wndWidth, wndHeight, 0.0f, 1.0f);

    Vertex v[] =
    {
        Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 1.0f),
        Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
        Vertex(1.0f, -1.0f, 1.0f, 1.0f, 1.0f),
        Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f),
    };

    auto hr = this->vertexBuffer.Initialize(this->device.Get(), v, ARRAYSIZE(v));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create vertex buffer.");
        return false;
    }

    DWORD indices[] =
    {
        0, 1, 2, 3
    };

    hr = this->indexBuffer.Initialize(this->device.Get(), indices, ARRAYSIZE(indices));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create index buffer.");
        return false;
    }

    //Texture load example
    hr = DirectX::CreateWICTextureFromFile(this->device.Get(), L"Res\\Textures\\EZ.png", nullptr, this->myTexture.GetAddressOf());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Could not load texture.");
        return false;
    }

    hr = this->constantBuffer.Initialize(this->device.Get(), this->deviceContext.Get());
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create constant buffer.");
        return false;
    }

    return true;
}