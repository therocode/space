#include "tasklogic.hpp"
#include "../roomutil.hpp"

TaskLogic::TaskLogic(const WallMap& walls, tsk::TRoomTask& tRoomTask, tsk::TWallTask& tWallTask, tsk::TDoorTask& tDoorTask):
    mWalls(walls),
    mTRoomTask(tRoomTask),
    mTWallTask(tWallTask),
    mTDoorTask(tDoorTask)
{
    (void)mTDoorTask;
}

void TaskLogic::update()
{
    updateRoomTasks();
    updateWallTasks();
}

void TaskLogic::updateRoomTasks()
{
    eraseIf([&] (int32_t id, const RoomTask& roomTask)
    {
        bool finished = true;
        forEachWall(roomTask.position, roomTask.size, [&]  (const glm::ivec2& coordinate, Orientation orientation)
        {
            if(!mWalls.at(coordinate, orientation))
                finished = false;
        });       

        return finished;
    }, mTRoomTask);
}

void TaskLogic::updateWallTasks()
{
    eraseIf([&] (int32_t id, const WallTask& wallTask)
    {
        return mWalls.at(wallTask.position, wallTask.orientation);
    }, mTWallTask);
}
