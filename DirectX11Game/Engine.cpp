#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    if (!this->render_window.Initialize(this, hInstance, window_title, window_class, width, height))
    {
        ErrorLogger::Log("Could not initialize window");
        return false;
    }

    if (!graphics.Initialize(this->render_window.GetHWND(), width, height))
    {
        ErrorLogger::Log("Could not initialize graphics");
        return false;
    }

    return true;
}

bool Engine::ProcessMessages()
{
    return this->render_window.ProcessMessages();
}

void Engine::Update()
{
    while (!keyboard.CharBufferIsEmpty())
    {
        UCHAR ch = keyboard.ReadChar();
    }

    while (!keyboard.KeyBufferIsEmpty())
    {
        KeyboardEvent e = keyboard.ReadKey();
        UCHAR keycode = e.GetKeyCode();
    }

    while (!mouse.EventBufferIsEmpty())
    {
        MouseEvent e = mouse.ReadEvent();

        if (e.GetType() == MouseEvent::EventType::RAW_MOVE)
        {
            std::string str = "X: ";
            str += std::to_string(e.GetPosX());
            str += ", ";
            str += "Y: ";
            str += std::to_string(e.GetPosY());
            str += "\n";
            OutputDebugStringA(str.c_str());
        }
    }
}

void Engine::RenderFrame()
{
    graphics.RenderFrame();
}
