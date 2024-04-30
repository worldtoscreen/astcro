#pragma once
#include <iostream>
#include <vector>

#include "imgui.h"
#include "col.h"

class Line
{
public:

    int x1, y1;
    int x2, y2;

    int color[3] = {255, 255, 255};

    void render()
    {
        ImGui::GetBackgroundDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), RenderColors::getLine(), 1.2f);
    }
};

