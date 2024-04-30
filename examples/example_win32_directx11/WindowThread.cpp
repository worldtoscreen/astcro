#include "WindowThread.h"

void WindowThread::update()
{
    while (true)
    {
        if (!this->hwnd)
        {
            this->hwnd = FindWindowA(NULL, "Roblox");
            if (this->hwnd == 0)
                break;
        }

        if (GetWindowRect(this->hwnd, &this->rect))
        {
            this->windowSize.x = this->rect.right - this->rect.left;
            this->windowSize.y = this->rect.bottom - this->rect.top;
            this->windowPos.x = this->rect.left;
            this->windowPos.y = this->rect.top;
        }
    }
}
