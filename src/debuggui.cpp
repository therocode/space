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

    void addTableInfo(const IdSet& idSet, th::Optional<int32_t>& clickedId)
    {
        if(ImGui::CollapsingHeader(idSet.meta.name.c_str()))
        {
            text(("Name: " + idSet.meta.name).c_str());

            if(idSet.meta.description)
            {
                text(("Description: " + *idSet.meta.description).c_str());
            }

            text(("Entries: " + std::to_string(idSet.ids.size())).c_str());
            text(idSet.meta.sorted ? "sorted" : "unsorted");

            bool showData = false;

            MemoryInfo memory = memoryInfo(idSet);
            text("Memory: " + formatMemory(memory.dataSize) + " (" + formatMemory(memory.totalSize) + ")");

            ImGui::Checkbox(("Show data##" + idSet.meta.name).c_str(), &showData);
            if(showData)
            {
                ImGui::Begin((idSet.meta.name + " data").c_str());

                AccessPatternInfo accessPattern = accessPatternInfo(idSet);
                text("Access pattern:");
                text(std::string("Random access: ") + std::to_string(accessPattern.percent[AccessType::RandomAccess] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::RandomAccess]));
                text(std::string("Iteration: ") + std::to_string(accessPattern.percent[AccessType::Iteration] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Iteration]));
                text(std::string("Addition: ") + std::to_string(accessPattern.percent[AccessType::Addition] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Addition]));
                text(std::string("Deletion: ") + std::to_string(accessPattern.percent[AccessType::Deletion] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Deletion]));
                text(std::string("Sort: ") + std::to_string(accessPattern.percent[AccessType::Sort] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Sort]));
                ImGui::Separator();
                if(!idSet.ids.empty())
                {
                    ImGui::Columns(1, "mycolumns");
                    ImGui::Separator();
                    text("ID");// ImGui::NextColumn();
                    ImGui::Separator();

                    TextList debugTexts;
                    bool colorPushed = false;
                    for(size_t i = 0; i < idSet.ids.size(); ++i)
                    {
                        if(clickedId && *clickedId == idSet.ids[i])
                        {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f,1.0f,0.0f,1.0f));
                            colorPushed = true;
                        }
                        if(ImGui::SmallButton((std::to_string(idSet.ids[i]) + "##" + idSet.meta.name).c_str()))
                        {
                            clickedId = idSet.ids[i];
                        }// ImGui::NextColumn();
                        if(colorPushed)
                        {
                            ImGui::PopStyleColor(1);
                            colorPushed = false;
                        }
                    }
                }
                else
                    text("No entries in set");
                ImGui::End();
            }
        }
    }

    void showInspector(const glm::vec2& position, const Zones& zones, const Grid<Gases>& atmosphere)
    {
        glm::ivec2 tile = static_cast<glm::ivec2>(position/32.0f);
        const Gases& gases = atmosphere.at(tile);
        ImGui::Begin("Inspector");

        text(std::to_string(position.x) + ", " + std::to_string(position.y) + " (" + std::to_string(tile.x) + ", " + std::to_string(tile.y) + std::string(")"));
        ImGui::Separator();
        text("Atmosphere");
        text("Zone: " + std::to_string(zones.zones.at(tile)));
        text("Pressure: " + std::to_string(pressurePercent(gases) * 100.0f) + "%");
        text("Gases: " + std::to_string(zones.zones.at(tile)));
        float total = static_cast<float>(pressure(gases));
        ImGui::BulletText("%s", ("Nitrogen: " + std::to_string(gases[Nitrogen] / total * 100.0f) + "% - " + std::to_string(gases[Nitrogen])).c_str());
        ImGui::BulletText("%s", ("Oxygen: " + std::to_string(gases[Oxygen] / total * 100.0f) + "% - " + std::to_string(gases[Oxygen])).c_str());
        ImGui::BulletText("%s", ("Carbon Dioxide: " + std::to_string(gases[CarbonDioxide] / total * 100.0f) + "% - " + std::to_string(gases[CarbonDioxide])).c_str());

        ImGui::End();
    }
}
