#pragma once
#include "ErrorLogger.h"
#include <vector>

#include <d3d11.h>

class AdapterData
{
public:
    AdapterData(IDXGIAdapter* pAdapter) : pAdapter(pAdapter) { };
    IDXGIAdapter* pAdapter = nullptr;
    DXGI_ADAPTER_DESC description;
};

class AdapterReader
{
public:
    static std::vector<AdapterData> GetAdapterData();
private:
    static std::vector<AdapterData> adapters;
};

