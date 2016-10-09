#include "imguiutil.hpp"
#include <imgui.h>

bool SmallDisableButton(const std::string& text, bool disabled)
{
    if(disabled)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1.0f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1.0f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1.0f, 0.0f, 0.5f));
    }
    bool clicked = ImGui::SmallButton(text.c_str());
    if(disabled)
    {
        ImGui::PopStyleColor(3);
    }

    return clicked && !disabled;
}
