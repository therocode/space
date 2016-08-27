#include "tasklogic.hpp"
#include "../roomutil.hpp"
#include "../taskutil.hpp"

TaskLogic::TaskLogic(GameData& data, const WallMap& walls):
    mData(data),
    mWalls(walls)
{
}

void TaskLogic::update()
{
    updateRoomTasks();
    updateWallTasks();
    updateDoorTasks();
}

void TaskLogic::updateRoomTasks()
{
    std::vector<int32_t> toErase;

    forEach([&] (int32_t id, const RoomTask& roomTask)
    {
        bool finished = true;
        forEachWall(roomTask.position, roomTask.size, [&]  (const glm::ivec2& coordinate, Orientation orientation)
        {
            if(!mWalls.at(coordinate, orientation))
            {
                finished = false;
            }
        });       

        if(finished)
        {
            toErase.push_back(id);
        }
    }, mData.tRoomTask);

    for(int32_t id : toErase)
        eraseTask(id, mData.tRoomTask, mData.unassignedTasks, mData.tAssignedTask);
}

void TaskLogic::updateWallTasks()
{
    std::vector<int32_t> toErase;

    forEach([&] (int32_t id, const WallTask& wallTask)
    {
        bool finished = mWalls.at(wallTask.position, wallTask.orientation);
        if(finished)
        {
            toErase.push_back(id);
        }
    }, mData.tWallTask);

    for(int32_t id : toErase)
        eraseTask(id, mData.tWallTask, mData.unassignedTasks, mData.tAssignedTask);
}

void TaskLogic::updateDoorTasks()
{
    std::vector<int32_t> toErase;

    forEach([&] (int32_t id, const DoorTask& doorTask)
    {
        bool finished = !findOne([&] (int32_t doorId, const Door& door)
        {
            return doorTask.position == door.position && doorTask.orientation == door.orientation;
        },  mData.tDoor).isNull();
        if(finished)
        {
            toErase.push_back(id);
        }
    }, mData.tDoorTask);

    for(int32_t id : toErase)
        eraseTask(id, mData.tDoorTask, mData.unassignedTasks, mData.tAssignedTask);
}
