#include "doorutil.hpp"
#include "wallutil.hpp"
#include "gases.hpp"
#include "atmosphereutil.hpp"

void createDoor(Door door, GameData& data)
{
    int32_t doorId = insert(std::move(door), data.tDoor).id;
    closeDoor(doorId, data);
}

void removeDoorAt(const WallPosition& position, GameData& data)
{
    auto door = doorAt(position, data);

    if(door)
    {
        int32_t toRemove = *door;
        erase(toRemove, data.tDoor);
        erase(toRemove, data.openDoors);
        erase(toRemove, data.lockedDoors);
        setWall(position, 1, data);
    }
}

void toggleDoor(int32_t doorId, GameData& data)
{
    if(has(doorId, data.openDoors))
        closeDoor(doorId, data);
    else
        openDoor(doorId, data);
}

void closeDoor(int32_t doorId, GameData& data)
{
    const auto& door = get(doorId, data.tDoor);
    erase(doorId, data.openDoors);
    setWall(door.position, 1, data);
}

void openDoor(int32_t doorId, GameData& data)
{
    if(!has(doorId, data.lockedDoors))
    {
        const auto& door = get(doorId, data.tDoor);
        insert(doorId, data.openDoors);
        setWall(door.position, 0, data);
    }
}

bool lockDoor(int32_t doorId, GameData& data)
{
    if(!has(doorId, data.lockedDoors))
    {
        if(has(doorId, data.openDoors))
            closeDoor(doorId, data);
        insert(doorId, data.lockedDoors);
        return true;
    }

    return false;
}

void unlockDoor(int32_t doorId, GameData& data)
{
    erase(doorId, data.lockedDoors);
}

int64_t doorPressureDiff(int32_t doorId, GameData& data)
{
    const Door& door = get(doorId, data.tDoor);
    const glm::ivec2& start = door.position.position;
    const glm::ivec2& end = start + (door.position.orientation == Orientation::Horizontal ? glm::ivec2(0, -1) : glm::ivec2(-1, 0));

    int64_t startPressure = pressure(data.atmosphere.at(start));
    int64_t endPressure = pressure(data.atmosphere.at(end));

    return startPressure - endPressure;
}

th::Optional<int32_t> doorAt(const WallPosition& position, GameData& data)
{
    const auto& found = findOne([&] (int32_t id, const Door& door)
    {
        return door.position == position;
    }, data.tDoor);


    if(found)
    {
        return {found->id};
    }
    else
        return {};
}

th::Optional<int32_t> lockedDoorAt(const WallPosition& position, GameData& data)
{
    auto door = doorAt(position, data);

    if(door && has(*door, data.lockedDoors))
    {
        return {*door};
    }
    else
        return {};
}

bool hasDoor(const WallPosition& position, const GameData& data)
{
    return static_cast<bool>(findOne([&](int32_t id, const Door& door)
    {
        return door.position == position;
    }, data.tDoor));
}

bool doorBordersZone(int32_t doorId, int32_t zoneId, GameData& data)
{
    const WallPosition& position = get(doorId, data.tDoor).position;
    return data.zones.zones.at(position.position) == zoneId || data.zones.zones.at(otherSide(position)) == zoneId;
}
