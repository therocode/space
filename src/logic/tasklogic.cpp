#include "tasklogic.hpp"
#include "../roomutil.hpp"
#include "../taskutil.hpp"

TaskLogic::TaskLogic(const WallMap& walls, tsk::TRoomTask& tRoomTask, tsk::TWallTask& tWallTask, tsk::TDoorTask& tDoorTask, IdSet& unassignedTasks, tsk::TAssignedTask& tAssignedTask):
    mWalls(walls),
    mTRoomTask(tRoomTask),
    mTWallTask(tWallTask),
    mTDoorTask(tDoorTask),
    mUnassignedTasks(unassignedTasks),
    mTAssignedTask(tAssignedTask)
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
    }, mTRoomTask);

    for(int32_t id : toErase)
        eraseTask(id, mTRoomTask, mUnassignedTasks, mTAssignedTask);
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
    }, mTWallTask);

    for(int32_t id : toErase)
        eraseTask(id, mTWallTask, mUnassignedTasks, mTAssignedTask);
}
