#include "tasklogic.hpp"
#include "../roomutil.hpp"
#include "../taskutil.hpp"

TaskLogic::TaskLogic(GameData& data):
    mData(data)
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
        forEachWall(roomTask.position, roomTask.size, [&]  (const WallPosition& position)
        {
            if(!mData.walls.at(position))
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
        eraseTask(id, mData.tRoomTask, mData);
}

void TaskLogic::updateWallTasks()
{
    std::vector<int32_t> toErase;

    forEach([&] (int32_t id, const WallTask& wallTask)
    {
        bool finished = mData.walls.at(wallTask.position);
        if(finished)
        {
            toErase.push_back(id);
        }
    }, mData.tWallTask);

    for(int32_t id : toErase)
        eraseTask(id, mData.tWallTask, mData);
}

void TaskLogic::updateDoorTasks()
{
    std::vector<int32_t> toErase;

    forEach([&] (int32_t id, const DoorTask& doorTask)
    {
        bool finished = !findOne([&] (int32_t doorId, const Door& door)
        {
            return doorTask.position == door.position;
        },  mData.tDoor).isNull();
        if(finished)
        {
            toErase.push_back(id);
        }
    }, mData.tDoorTask);

    for(int32_t id : toErase)
        eraseTask(id, mData.tDoorTask, mData);
}
