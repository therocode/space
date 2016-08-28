#include "structuregui.hpp"
#include "structuretypes.hpp"
#include <imgui.h>

void showStructureGui(int32_t structureId, int32_t structureTypeId, GameData& data)
{
    const StructureType& type = get(structureTypeId, data.tStructureType);

    ImGui::Begin(type.name.c_str());
    ImGui::Text("%s", "heya");
    ImGui::End();
}
