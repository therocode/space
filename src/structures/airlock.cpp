#include "airlock.hpp"
#include "../structureutil.hpp"
#include "../doorutil.hpp"

void discoverAirlockDoors(int32_t id, const Structure& structure, GameData& data)
{
    std::vector<int32_t>& doors = get(id, data.tAirlock).doors;
    const auto& tile = structure.position;

    forEach([&] (int32_t doorId, const Door& door)
    {
        if(door.position.position == tile)
        {
            doors.push_back(doorId);
        }
        else if(door.position.position == tile + glm::ivec2(1, 0) && door.position.orientation == Orientation::Vertical)
        {
            doors.push_back(doorId);
        }
        else if(door.position.position == tile + glm::ivec2(0, 1) && door.position.orientation == Orientation::Horizontal)
        {
            doors.push_back(doorId);
        }
    }, data.tDoor);
}

void requestMode(int32_t id, Airlock::Mode mode, GameData& data)
{
    const Structure& structure = get(id, data.tStructure);
    Airlock& airlock = get(id, data.tAirlock);
    if(!airlock.exit)
        return;

    if(airlock.currentMode != mode && airlock.currentMode != Airlock::Pumping)
    {
        airlock.currentMode = Airlock::Pumping;

        startPumpDoor(id, mode, mode == Airlock::In ? Airlock::Out : Airlock::In, data);
    }   
}

void startPumpDoor(int32_t id, Airlock::Mode targetMode, Airlock::Mode pumpMode, GameData& data)
{
    const Structure& structure = get(id, data.tStructure);
    const Airlock& airlock = get(id, data.tAirlock);
    const Door& pumpingDoor = pumpMode == Airlock::In ? get(airlock.doors[0], data.tDoor) : get(*airlock.exit, data.tDoor);

    glm::ivec2 start = structure.position;
    glm::ivec2 end;
    if(pumpingDoor.position.position != start)
        end = pumpingDoor.position.position;
    else
        end = otherSide(pumpingDoor);

    if(targetMode == pumpMode)
        std::swap(start, end);

    AirlockActivity* existing = findOne(id, data.tAirlockActivity);
    if(!existing)
    {
        int32_t leakId = insert(ZoneLeak{start, end, 400, 10000}, data.tZoneLeak);
        insert(id, AirlockActivity{targetMode, pumpMode, leakId}, data.tAirlockActivity); 
    }
    else
    {
        erase(existing->leakId, data.tZoneLeak);
        int32_t leakId = insert(ZoneLeak{start, end, 400, 10000}, data.tZoneLeak);
        existing->currentlyPumping = pumpMode;
    }
}

void requestOpenAirlockDoor(int32_t id, int32_t doorId, GameData& data)
{
    const Airlock& airlock = get(id, data.tAirlock);
    if(!airlock.exit)
        return;

    TH_ASSERT(std::find(airlock.doors.begin(), airlock.doors.end(), doorId) != airlock.doors.end(), "Invalid doorId given to airlock");
    Airlock::Mode targetMode = *airlock.exit == doorId ? Airlock::Out : Airlock::In;

    if(airlock.currentMode == targetMode)
    {
        openDoor(doorId, data);
    }
    else if(airlock.currentMode == Airlock::Pumping)
    {
        return;
    }
    else
    {
        requestMode(id, targetMode, data);
    }
}

void airlockUpdate(GameData& data)
{
    eraseIf([&] (int32_t id, AirlockActivity& airlockActivity)
    {
        Airlock& airlock = get(id, data.tAirlock);
        const Structure& structure = get(id, data.tStructure);

        if(airlockActivity.targetMode != airlockActivity.currentlyPumping)
        {
            int64_t currentPressure = pressure(data.atmosphere.at(structure.position));

            if(currentPressure == 0)
            {
                startPumpDoor(id, airlockActivity.targetMode, airlockActivity.targetMode, data);
            }
        }
        else
        {
            const glm::ivec2& otherSide = get(airlockActivity.leakId, data.tZoneLeak).start;
            int64_t pressureDifference = pressure(data.atmosphere.at(otherSide)) - pressure(data.atmosphere.at(structure.position));

            if(pressureDifference < 5)
            {
                erase(airlockActivity.leakId, data.tZoneLeak);
                airlock.currentMode = airlockActivity.targetMode;
                return true;
            }
        }
        return false;
    }, data.tAirlockActivity);

    forEach([&] (int32_t id, const Airlock& airlock)
    {
        if(airlock.currentMode == Airlock::In)
        {
            for(int32_t door : airlock.doors)
                unlockDoor(door, data);

            if(airlock.exit)
                structureLockDoor(id, *airlock.exit, data);
        }
        else if(airlock.currentMode == Airlock::Out)
        {
            for(int32_t door : airlock.doors)
            {
                if(airlock.exit && door != *airlock.exit)
                    structureLockDoor(id, door, data);
            }

            if(airlock.exit)
                unlockDoor(*airlock.exit, data);
        }
        else
        {
            for(int32_t door : airlock.doors)
                structureLockDoor(id, door, data);
        }

    }, data.tAirlock);
}

th::Optional<int32_t> airlockProvidesPath(int32_t structureId, const WallPosition& wallPosition, const GameData& data)
{
    const Airlock& airlock = get(structureId, data.tAirlock);

    if(std::find_if(airlock.doors.begin(), airlock.doors.end(), [&] (int32_t doorId)
    {
        return get(doorId, data.tDoor).position == wallPosition;
    }) != airlock.doors.end())
    {
        return {100};
    }
    else
        return {};
}
