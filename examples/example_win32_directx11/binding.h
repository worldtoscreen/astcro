#pragma once
#define IMGUI_DEFINE_MATH_OPERATORS

#include "imgui.h"
#include "imgui_internal.h"

#include <iostream>
#include "map"

namespace ImGui
{
    void HotKey(const char* label, short& key, ImVec2& size_arg);
}
