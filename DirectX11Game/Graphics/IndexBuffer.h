#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include <memory>

class IndexBuffer
{
private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    UINT bufferSize = 0;

public:
    IndexBuffer() {}
    
    ID3D11Buffer* Get() const
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return buffer.GetAddressOf();
    }

    UINT BufferSize() const
    {
        return this->bufferSize;
    }

    HRESULT Initialize(ID3D11Device* device, DWORD* data, UINT numIndicies)
    {
        this->bufferSize = numIndicies;

        D3D11_BUFFER_DESC indexBufferDesc;
        ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = sizeof(DWORD) * numIndicies;
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = NULL;
        indexBufferDesc.MiscFlags = NULL;

        D3D11_SUBRESOURCE_DATA indexBufferData;
        indexBufferData.pSysMem = data;

        auto hr = device->CreateBuffer(&indexBufferDesc, &indexBufferData, this->buffer.GetAddressOf());
        return hr;
    }
};

