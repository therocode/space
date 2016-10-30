#include "airlock.hpp"
#include "../structureutil.hpp"
#include "../doorutil.hpp"
#include "../wallutil.hpp"
#include "../atmosphereutil.hpp"

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

void autoDetectAirlockMode(int32_t id, GameData& data)
{
    Airlock& airlock = get(id, data.tAirlock);

    airlock.exit = {};
    for(int32_t doorId : airlock.doors)
    {
        if(doorBordersZone(doorId, 0, data))
        {
            if(!airlock.exit)
                airlock.exit = doorId;
            else
            {
                airlock.exit = {};
                break;
            }
        }
    }

    if(airlock.exit)
    {
        if(healthyAtmosphere(data.atmosphere.at(get(id, data.tStructure).position)))
        {
            airlock.currentMode = AirlockMode::In;
        }
        else
        {
            airlock.currentMode = AirlockMode::Out;
        }
    }
}

void requestMode(int32_t id, AirlockMode mode, GameData& data)
{
    const Structure& structure = get(id, data.tStructure);
    Airlock& airlock = get(id, data.tAirlock);
    if(!airlock.exit || airlock.coolDown > 0)
        return;

    if(airlock.currentMode != mode && airlock.currentMode != AirlockMode::Pumping)
    {
        airlock.currentMode = AirlockMode::Pumping;

        startPumpDoor(id, mode, mode == AirlockMode::In ? AirlockMode::Out : AirlockMode::In, data);
    }   
}

void startPumpDoor(int32_t id, AirlockMode targetMode, AirlockMode pumpMode, GameData& data)
{
    const Structure& structure = get(id, data.tStructure);
    const Airlock& airlock = get(id, data.tAirlock);
    const Door& pumpingDoor = pumpMode == AirlockMode::In ? get(airlock.doors[0], data.tDoor) : get(*airlock.exit, data.tDoor);

    glm::ivec2 start = structure.position;
    glm::ivec2 end;
    if(pumpingDoor.position.position != start)
        end = pumpingDoor.position.position;
    else
        end = otherSide(pumpingDoor.position);

    if(targetMode == pumpMode)
        std::swap(start, end);

    AirlockActivity* existing = findOne(id, data.tAirlockActivity);
    if(!existing)
    {
        int32_t leakId = insert(ZoneLeak{start, end, 400, 10000}, data.tZoneLeak).id;
        insert(id, AirlockActivity{targetMode, pumpMode, leakId}, data.tAirlockActivity); 
    }
    else
    {
        erase(existing->leakId, data.tZoneLeak);
        int32_t leakId = insert(ZoneLeak{start, end, 400, 10000}, data.tZoneLeak).id;
        existing->currentlyPumping = pumpMode;
        existing->leakId = leakId;
    }
}

void requestOpenAirlockDoor(int32_t id, int32_t doorId, GameData& data)
{
    const Airlock& airlock = get(id, data.tAirlock);
    if(!airlock.exit)
        return;

    TH_ASSERT(std::find(airlock.doors.begin(), airlock.doors.end(), doorId) != airlock.doors.end(), "Invalid doorId given to airlock: " << doorId);
    AirlockMode targetMode = *airlock.exit == doorId ? AirlockMode::Out : AirlockMode::In;

    if(airlock.currentMode == targetMode)
    {
        openDoor(doorId, data);
    }
    else if(airlock.currentMode == AirlockMode::Pumping)
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
                airlock.coolDown = 120;
                return true;
            }
        }
        return false;
    }, data.tAirlockActivity);

    forEach([&] (int32_t id, Airlock& airlock)
    {
        if(airlock.coolDown > 0)
            --airlock.coolDown;

        if(airlock.currentMode == AirlockMode::In)
        {
            for(int32_t door : airlock.doors)
                unlockDoor(door, data);

            if(airlock.exit)
                structureLockDoor(id, *airlock.exit, data);
        }
        else if(airlock.currentMode == AirlockMode::Out)
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
