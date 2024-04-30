#include "binding.h"

void ImGui::HotKey(const char* label, short& key, ImVec2& size_arg)
{
    static std::map<const char*, bool> keyDownMap;

    if (keyDownMap.find(label) == keyDownMap.end())
    {
        keyDownMap[label] = false;
    }

    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    ImVec2 pos = window->DC.CursorPos;

    ImGui::Text(label);
    pos.x += label_size.x + style.ItemInnerSpacing.x;
    ImGui::SameLine();

    ImVec2 size = CalcItemSize(size_arg, label_size.x + style.FramePadding.x * 2.0f, label_size.y + style.FramePadding.y * 2.0f);

    const ImRect bb(pos, pos + size);
    ItemSize(size, style.FramePadding.y);
    if (!ItemAdd(bb, id))
        return; // Unable to add widget

    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held);

    if (pressed) keyDownMap[label] = !keyDownMap[label];

    const ImU32 col = GetColorU32((held && hovered) ? ImGuiCol_ButtonActive : hovered ? ImGuiCol_ButtonHovered : ImGuiCol_Button);
    RenderNavHighlight(bb, id);
    RenderFrame(bb.Min, bb.Max, col, true, style.FrameRounding);

    if (g.LogEnabled)
        LogSetNextTextDecoration("[", "]");

    const char* dec = "...";

    if (keyDownMap[label]) {
        for (int i = 0; i < 256; i++)
        {
            if (g.IO.KeysDown[i])
            {
                key = i;
                dec = ImGui::GetKeyName(ImGuiKey(i));
                keyDownMap[label] = false;
                break;
            }
        }
    }
    else
    {
        dec = ImGui::GetKeyName(ImGuiKey(key));
    }

    RenderTextClipped(bb.Min + style.FramePadding, bb.Max - style.FramePadding, dec, NULL, &label_size, style.ButtonTextAlign, &bb);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return;
}
