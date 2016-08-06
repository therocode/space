#include "debuggui.hpp"

namespace DebugGui
{
    void text(const std::string& string)
    {
        ImGui::Text("%s", string.c_str());
    }

    std::string formatMemory(size_t memorySize)
    {
        if(memorySize < 1024)
            return std::to_string(memorySize) + "B";
        else if(memorySize < 1024 * 1024)
            return std::to_string(memorySize / 1024) + "kB";
        else
            return std::to_string(memorySize / (1024 * 1024)) + "MB";
    }
}
