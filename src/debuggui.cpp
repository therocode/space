#include "debuggui.hpp"

namespace DebugGui
{
    void text(const std::string& string)
    {
        ImGui::Text("%s", string.c_str());
    }
}
