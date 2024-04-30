#pragma once
#include <iostream>
#include <vector>

#include "imgui.h"
#include "col.h"

class Circle
{
public:
    int centerX, centerY;
    float dx, dy;

    bool renderable = true;

    float distance(Circle* other)
    {
        return sqrt((centerX - other->centerX) * (centerX - other->centerX) + (centerY - other->centerY) * (centerY - other->centerY));
    }

    void render()
    {
        if (!this->renderable)
            return;

        ImGui::GetBackgroundDrawList()->AddCircleFilled(ImVec2(centerX, centerY), 4, RenderColors::getCircle());
    }
};

