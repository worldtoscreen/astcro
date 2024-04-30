#pragma once
#include "imgui.h"

enum tab
{
    main_page,
    visuals,
    macrosettings,
    colors
};

namespace Colors
{
    tab current_tab = tab::main_page;

    static ImColor Background = ImColor(7, 8, 13, 255);
    static ImColor Inactive = ImColor(19, 30, 54, 255);
    static ImColor Active = ImColor(59, 75, 168, 255);
    static ImColor Text = ImColor(255, 255, 255, 255);

    void style()
    {
        ImGuiStyle& style = ImGui::GetStyle();


        style.Colors[ImGuiCol_WindowBg] = Background;
        style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
        style.Colors[ImGuiCol_TitleBg] = Active;
        style.Colors[ImGuiCol_TitleBgActive] = Active;
        style.Colors[ImGuiCol_TitleBgCollapsed] = Active;
        style.Colors[ImGuiCol_FrameBg] = Inactive;
        style.Colors[ImGuiCol_FrameBgHovered] = Active;
        style.Colors[ImGuiCol_FrameBgActive] = Active;

        style.Colors[ImGuiCol_Button] = Inactive;
        style.Colors[ImGuiCol_ButtonHovered] = Active;
        style.Colors[ImGuiCol_ButtonActive] = Active;

        style.Colors[ImGuiCol_Header] = Active;
        style.Colors[ImGuiCol_HeaderHovered] = Active;
        style.Colors[ImGuiCol_HeaderActive] = Active;

        style.Colors[ImGuiCol_CheckMark] = Active;
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 0.00f);
        style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.24f, 0.52f, 0.88f, 0.00f);

        style.Colors[ImGuiCol_Text] = Text;
    }
}

namespace config
{
    bool vsync = true;

    bool checkbox1 = false;
    bool checkbox2 = false;
    bool checkbox3 = false;
    bool checkbox4 = false;

    int intValue = 10;

    float particleInterpolation = 35.f;
    bool particles = false;
    bool move = true;
    bool output = false;
}
