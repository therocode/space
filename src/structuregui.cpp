#include "structuregui.hpp"
#include "structuretypes.hpp"
#include <imgui.h>

bool showStructureGui(int32_t structureId, int32_t structureTypeId, GameData& data)
{
    bool close = false;

    const StructureType& type = get(structureTypeId, data.tStructureType);

    ImGui::Begin(type.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if(structureTypeId == Airlock)
    {
        ImGui::Text("%s", "This airlock is neat, but it's not functional");
    }
    else if(structureTypeId == CryoPods)
    {
        ImGui::Text("%s", "Frozen people exist inside. They are all dead now");
    }
    else if(structureTypeId == Battery)
    {
        ImGui::Text("%s", "Nice, a battery with infinite power!");
    }
    else if(structureTypeId == Crate)
    {
        ImGui::Text("%s", "No one knows what this crate contains since it is closed");
    }
    else if(structureTypeId == Toilet)
    {
        ImGui::Text("%s", "This toilet is not equipped with toilet paper");
    }

    if(ImGui::SmallButton("Close"))
    {
        close = true;
    }

    ImGui::End();

    return close;
}
