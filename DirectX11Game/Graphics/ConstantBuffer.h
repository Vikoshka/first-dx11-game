#pragma once

#include <d3d11.h>
#include <wrl/client.h>
#include "../Common/ErrorLogger.h"
#include "ConstantBufferTypes.h"

template<class T>
class ConstantBuffer
{
private:
    ConstantBuffer(const ConstantBuffer<T>& rhs);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    ID3D11DeviceContext* deviceContext = nullptr;

public:
    ConstantBuffer() {}

    T data;

    ID3D11Buffer* Get() const
    {
        return buffer.Get();
    }

    ID3D11Buffer* const* GetAddressOf() const
    {
        return buffer.GetAddressOf();
    }

    HRESULT Initialize(ID3D11Device *device, ID3D11DeviceContext* deviceContext)
    {
        this->deviceContext = deviceContext;
        ZeroMemory(&data, sizeof(T));

        //initialize constant buffer
        D3D11_BUFFER_DESC desc;
        desc.Usage = D3D11_USAGE_DYNAMIC;
        desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        desc.MiscFlags = 0;
        desc.ByteWidth = static_cast<UINT>(sizeof(T) + (16 - (sizeof(T) % 16)));
        desc.StructureByteStride = 0;

        auto hr = device->CreateBuffer(&desc, 0, this->buffer.GetAddressOf());
        return hr;
    }

    bool ApplyChanges()
    {
        D3D11_MAPPED_SUBRESOURCE mpSubRes;
        auto hr = this->deviceContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mpSubRes);
        if (FAILED(hr))
        {
            ErrorLogger::Log(hr, "Failed to map buffer.");
            return false;
        }
        CopyMemory(mpSubRes.pData, &data, sizeof(T));
        this->deviceContext->Unmap(buffer.Get(), 0);

        return true;
    }
};
