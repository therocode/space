#pragma once
#include <data.hpp>

int32_t createItemInContainer(Item item, int32_t containerId, GameData& data);
std::string toString(Item::Type type);
