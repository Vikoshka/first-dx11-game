#pragma once
#include "../Common/ErrorLogger.h"
#pragma comment(lib, "D3DCompiler.lib")
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <memory>

class VertexShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path, D3D11_INPUT_ELEMENT_DESC* layout_desc, SIZE_T numElements);
    ID3D11VertexShader* GetShader();
    ID3D10Blob* GetBuffer();
    ID3D11InputLayout* GetInputLayout();

private:
    Microsoft::WRL::ComPtr<ID3D11VertexShader> shader = nullptr;
    Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer = nullptr;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout = nullptr;

};

class PixelShader
{
public:
    bool Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path);
    ID3D11PixelShader* GetShader();
    ID3D10Blob* GetBuffer();

private:
    Microsoft::WRL::ComPtr<ID3D11PixelShader> shader = nullptr;
    Microsoft::WRL::ComPtr<ID3D10Blob> shader_buffer = nullptr;
};