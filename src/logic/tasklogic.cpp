#include "tasklogic.hpp"
#include "../roomutil.hpp"
#include "../taskutil.hpp"

TaskLogic::TaskLogic(TaskData& tsk, EntityData& ent, const WallMap& walls):
    mTsk(tsk),
    mEnt(ent),
    mWalls(walls)
{
    (void)mEnt;
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
    }, mTsk.tRoomTask);

    for(int32_t id : toErase)
        eraseTask(id, mTsk.tRoomTask, mTsk.unassignedTasks, mTsk.tAssignedTask);
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
    }, mTsk.tWallTask);

    for(int32_t id : toErase)
        eraseTask(id, mTsk.tWallTask, mTsk.unassignedTasks, mTsk.tAssignedTask);
}
