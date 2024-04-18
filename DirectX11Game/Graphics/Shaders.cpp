#include "Shaders.h"

bool VertexShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path, D3D11_INPUT_ELEMENT_DESC* layout_desc, SIZE_T numElements)
{
    HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), this->shader_buffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring err = L"Failed to load shader: ";
        err += shader_path;
        ErrorLogger::Log(hr, err);
        return false;
    }

    hr = device->CreateVertexShader(
        this->shader_buffer->GetBufferPointer(),
        this->shader_buffer->GetBufferSize(),
        NULL,
        this->shader.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring err = L"Failed to create vertex shader: ";
        err += shader_path;
        ErrorLogger::Log(hr, err);
        return false;
    }

    hr = device->CreateInputLayout(layout_desc, numElements, shader_buffer->GetBufferPointer(), shader_buffer->GetBufferSize(), inputLayout.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring err = L"Could not create input layout of shader: ";
        err += shader_path;
        ErrorLogger::Log(hr, err);
        return false;
    }

    return true;
}

ID3D11VertexShader* VertexShader::GetShader()
{
    return this->shader.Get();
}

ID3D10Blob* VertexShader::GetBuffer()
{
    return this->shader_buffer.Get();
}

ID3D11InputLayout* VertexShader::GetInputLayout()
{
    return this->inputLayout.Get();
}

bool PixelShader::Initialize(Microsoft::WRL::ComPtr<ID3D11Device>& device, std::wstring shader_path)
{
    HRESULT hr = D3DReadFileToBlob(shader_path.c_str(), this->shader_buffer.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring err = L"Failed to load shader: ";
        err += shader_path;
        ErrorLogger::Log(hr, err);
        return false;
    }

    hr = device->CreatePixelShader(
        this->shader_buffer->GetBufferPointer(),
        this->shader_buffer->GetBufferSize(),
        NULL,
        this->shader.GetAddressOf());
    if (FAILED(hr))
    {
        std::wstring err = L"Failed to create pixel shader: ";
        err += shader_path;
        ErrorLogger::Log(hr, err);
        return false;
    }

    return true;
}

ID3D11PixelShader* PixelShader::GetShader()
{
    return this->shader.Get();
}

ID3D10Blob* PixelShader::GetBuffer()
{
    return this->shader_buffer.Get();
}
