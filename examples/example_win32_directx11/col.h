#pragma once

namespace RenderColors
{
    float circleColor[3] = {255, 255, 255};
    float lineColor[3] = {255, 255, 255};

    ImColor getCircle()
    {
        return ImColor(circleColor[0], circleColor[1], circleColor[2]);
    }

    ImColor getLine()
    {
        return ImColor(lineColor[0], lineColor[1], lineColor[2]);
    }
}
