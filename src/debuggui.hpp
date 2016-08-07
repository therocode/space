#pragma once
#include <imgui.h>
#include <string>
#include <vector>
#include "data/memory.hpp"
#include "data/accesspattern.hpp"

namespace DebugGui
{
    using TextList = std::vector<std::vector<std::string>>;

    template<typename T>
    std::vector<std::string> debugHeaders(const T&) { return {};}

    void text(const std::string& string);

    std::string formatMemory(size_t memorySize);

    template <typename Table>
    void addTableInfo(const Table& table)
    {
        if(ImGui::CollapsingHeader(table.meta.name.c_str()))
        {
            text(("Name: " + table.meta.name).c_str());

            if(table.meta.description)
            {
                text(("Description: " + *table.meta.description).c_str());
            }

            text(("Entries: " + std::to_string(table.ids.size())).c_str());
            text(table.meta.sorted ? "sorted" : "unsorted");

            static bool showData = false;

            MemoryInfo memory = memoryInfo(table);
            text("Memory: " + formatMemory(memory.dataSize) + " (" + formatMemory(memory.totalSize) + ")");

            ImGui::Checkbox(("Show data##" + table.meta.name).c_str(), &showData);
            if(showData)
            {
                ImGui::Begin((table.meta.name + " data").c_str());

                AccessPatternInfo accessPattern = accessPatternInfo(table);
                text("Access pattern:");
                text(std::string("Random access: ") + std::to_string(accessPattern.percent[AccessType::RandomAccess] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::RandomAccess]));
                text(std::string("Iteration: ") + std::to_string(accessPattern.percent[AccessType::Iteration] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Iteration]));
                text(std::string("Addition: ") + std::to_string(accessPattern.percent[AccessType::Addition] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Addition]));
                text(std::string("Deletion: ") + std::to_string(accessPattern.percent[AccessType::Deletion] * 100.0f) + std::string("% - ") + std::to_string(accessPattern.total[AccessType::Deletion]));
                ImGui::Separator();
                if(!table.ids.empty())
                {
                    auto headers = debugHeaders(table);
                    int32_t dataColumnCount = static_cast<int32_t>(headers.size());

                    ImGui::Columns(dataColumnCount + 1, "mycolumns");
                    ImGui::Separator();
                    text("ID"); ImGui::NextColumn();
                    for(const auto& header : headers)
                    {
                        text(header); ImGui::NextColumn();
                    }
                    ImGui::Separator();

                    TextList debugTexts;
                    for(size_t i = 0; i < table.ids.size(); ++i)
                    {
                        text(std::to_string(table.ids[i])); ImGui::NextColumn();
                        debugText(table.data[i], debugTexts);

                        for(size_t cellIndex = 0; cellIndex < debugTexts.size(); ++cellIndex)
                        {
                            const auto& cellTextList = debugTexts[cellIndex];
                            if(cellTextList.size() == 1)
                            {
                                text(cellTextList.front()); ImGui::NextColumn();
                            }
                            else
                            {
                                auto firstText = cellTextList.front();

                                bool node_open = ImGui::TreeNode((firstText + "##" + std::to_string(table.ids[i]) + "_" + std::to_string(cellIndex)).c_str());
                                if(node_open)
                                {
                                    for(auto iter = cellTextList.begin() + 1; iter != cellTextList.end(); ++iter)
                                    {
                                        text(*iter);
                                    }

                                    ImGui::TreePop();
                                }

                                ImGui::NextColumn();
                            }
                        }
                    }
                }
                else
                    text("No entries in table");
                ImGui::End();
            }
        }
    }

    template <typename... Tables>
    void showDataTables(const Tables&... tables)
    {
        using swallow = int[];

        ImGui::Begin("Data tables");
        (void)swallow{(addTableInfo(tables), 0)...};
        ImGui::End();
    }
}