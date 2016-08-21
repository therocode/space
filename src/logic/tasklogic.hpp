#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class TaskLogic
{
    public:
        TaskLogic(TaskData& tsk, EntityData& ent, WorldData& wld, const WallMap& walls);
        void update();
    private:
        void updateRoomTasks();
        void updateWallTasks();
        void updateDoorTasks();

        TaskData& mTsk;
        EntityData& mEnt;
        WorldData& mWld;
        const WallMap& mWalls;
};
