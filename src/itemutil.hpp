#pragma once
#include <data.hpp>

int32_t createItemInContainer(Item item, int32_t containerId, GameData& data);
std::string toString(Item::Type type);
bool needsSpaceSuit(const Path& path, GameData& data);
bool hasSpaceSuit(int32_t actorId, GameData& data);
