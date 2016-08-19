#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class TaskLogic
{
    public:
        TaskLogic(TaskData& tsk, EntityData& ent, const WallMap& walls);
        void update();
    private:
        void updateRoomTasks();
        void updateWallTasks();

        TaskData& mTsk;
        EntityData& mEnt;
        const WallMap& mWalls;
};
