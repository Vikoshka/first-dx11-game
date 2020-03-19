#include "Engine.h"

bool Engine::Initialize(HINSTANCE hInstance, std::string window_title, std::string window_class, int width, int height)
{
    return this->render_window.Initialize(this, hInstance, window_title, window_class, width, height);
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
    }
}
