#include "Engine.h"


//HRESULT WinProc();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    //ErrorLogger::Log(S_OK, "Test Message");

    CoInitialize(NULL);

    Engine engine;
    if (engine.Initialize(hInstance, "Test", "test_class", 1024, 768))
    {
        while (engine.ProcessMessages() == true)
        {
            engine.Update();
            engine.RenderFrame();
        }
    }

    return 0;
}