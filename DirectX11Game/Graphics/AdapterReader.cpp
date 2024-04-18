#include "AdapterReader.h"

std::vector<AdapterData> AdapterReader::adapters;

std::vector<AdapterData> AdapterReader::GetAdapterData()
{
    if (adapters.size() > 0)
        return adapters;

    IDXGIFactory* pFactory = nullptr;
    HRESULT hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pFactory));
    if (FAILED(hr))
    {
        ErrorLogger::Log(hr, "Failed to create DXGIFactory.");
        exit(-1);
    }

    IDXGIAdapter* pAdapter = nullptr;
    UINT index = 0;
    while (SUCCEEDED(pFactory->EnumAdapters(index, &pAdapter)))
    {
        adapters.push_back(AdapterData(pAdapter));
        index++;
    }

    return adapters;
}
