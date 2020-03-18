#include "Engine.h"


//HRESULT WinProc();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{

    //ErrorLogger::Log(S_OK, "Test Message");

    Engine engine;
    engine.Initialize(hInstance, "Test", "test_class", 800, 600);

    while (engine.ProcessMessages() == true)
    {

    }

    return 0;
}