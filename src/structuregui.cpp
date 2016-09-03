#include "structuregui.hpp"
#include "structuretypes.hpp"
#include "doorutil.hpp"
#include "structures/airlock.hpp"
#include <imgui.h>

bool showStructureGui(int32_t structureId, int32_t structureTypeId, GameData& data)
{
    bool close = false;

    const StructureType& type = get(structureTypeId, data.tStructureType);

    ImGui::Begin(type.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if(structureTypeId == Structures::Airlock)
    {
        Airlock& airlock = get(structureId, data.tAirlock);
        ImGui::Text("%s", "This airlock is neat!");

        std::string mode = "In";
        if(airlock.currentMode == Airlock::Out)
            mode = "Out";
        else if(airlock.currentMode == Airlock::Pumping)
            mode = "Pumping";

        ImGui::Text("Current mode is %s", mode.c_str());
        ImGui::Text("Connected doors:");
        for(int32_t doorId : airlock.doors)
        {
            bool isExit = (doorId == airlock.exit);
            const Door& door = get(doorId, data.tDoor);
            if(ImGui::SmallButton((std::to_string(door.position.x) + "," + std::to_string(door.position.y) + (isExit ? std::string(" (exit)") : std::string()) + "###" + std::to_string(doorId)).c_str()))
            {
                airlock.exit = doorId;
            }

            ImGui::SameLine(150);
            if(ImGui::SmallButton((std::string("Open##") + std::to_string(doorId)).c_str()))
            {
                std::cout << "want to open: " << doorId << "\n";
                requestOpenAirlockDoor(structureId, doorId, data);
            }
        }
    }
    else if(structureTypeId == Structures::CryoPods)
    {
        ImGui::Text("%s", "Frozen people exist inside. They are all dead now");
    }
    else if(structureTypeId == Structures::Battery)
    {
        ImGui::Text("%s", "Nice, a battery with infinite power!");
    }
    else if(structureTypeId == Structures::Crate)
    {
        ImGui::Text("%s", "No one knows what this crate contains since it is closed");
    }
    else if(structureTypeId == Structures::Toilet)
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
