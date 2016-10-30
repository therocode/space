#include "structuregui.hpp"
#include "structuretypes.hpp"
#include "doorutil.hpp"
#include "itemutil.hpp"
#include "structures/airlock.hpp"
#include <imgui.h>

bool showStructureGui(int32_t structureId, int32_t structureTypeId, GameData& data)
{
    bool close = false;

    const StructureType& type = get(structureTypeId, data.tStructureType);

    ImGui::Begin(type.name.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if(structureTypeId == Structure::Airlock)
    {
        Airlock& airlock = get(structureId, data.tAirlock);
        ImGui::Text("%s", "This airlock is neat!");

        std::string mode = "In";
        if(airlock.currentMode == AirlockMode::Out)
            mode = "Out";
        else if(airlock.currentMode == AirlockMode::Pumping)
            mode = "Pumping";

        ImGui::Text("Current mode is %s", mode.c_str());
        ImGui::Text("Connected doors:");
        for(int32_t doorId : airlock.doors)
        {
            bool isExit = (doorId == airlock.exit);
            const Door& door = get(doorId, data.tDoor);
            if(ImGui::SmallButton((std::to_string(door.position.position.x) + "," + std::to_string(door.position.position.y) + (isExit ? std::string(" (exit)") : std::string()) + "###" + std::to_string(doorId)).c_str()))
            {
                airlock.exit = doorId;
            }

            ImGui::SameLine(150);
            if(ImGui::SmallButton((std::string("Open##") + std::to_string(doorId)).c_str()))
            {
                requestOpenAirlockDoor(structureId, doorId, data);
            }
        }
    }
    else if(structureTypeId == Structure::CryoPods)
    {
        ImGui::Text("%s", "Frozen people exist inside. They are all dead now");
    }
    else if(structureTypeId == Structure::Battery)
    {
        ImGui::Text("%s", "Nice, a battery with infinite power!");
    }
    else if(structureTypeId == Structure::Crate)
    {
        const Crate& crate = get(structureId, data.tCrate);
        int32_t containerId = crate.containerId;

        std::vector<std::string> contentStrings;
        int32_t itemCount = 0;

        forEach([&] (int32_t itemStoringId, const ItemStoring itemStoring)
        {
            if(itemStoring.containerId == containerId)
            {
                ++itemCount;
                const Item& item = get(itemStoring.itemId, data.tItem);
                contentStrings.push_back(toString(item.type));
            }
        }, data.tItemStoring);

        ImGui::Text("This is a magnificent crate with %d items in it", itemCount);
        for(const auto& contentString : contentStrings)
        {
            ImGui::Text("%s", contentString.c_str());
        }
    }
    else if(structureTypeId == Structure::Toilet)
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
