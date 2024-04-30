#pragma once

#include <Windows.h>
#include "imgui.h"

class WindowThread
{
public:
    ImVec2 windowPos = { 0, 0 };
    ImVec2 windowSize = { 1920, 1200 };

    void update();

    HWND hwnd;
    RECT rect;
};

