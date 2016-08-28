#include "airlock.hpp"

void discoverAirlockDoors(int32_t id, const Structure& structure, GameData& data)
{
    std::vector<int32_t>& doors = get(id, data.tAirlock).doors;
    const auto& tile = structure.position;

    forEach([&] (int32_t doorId, const Door& door)
    {
        if(door.position == tile)
        {
            doors.push_back(doorId);
        }
        else if(door.position == tile + glm::ivec2(1, 0) && door.orientation == Orientation::Vertical)
        {
            doors.push_back(doorId);
        }
        else if(door.position == tile + glm::ivec2(0, 1) && door.orientation == Orientation::Horizontal)
        {
            doors.push_back(doorId);
        }
    }, data.tDoor);
}
