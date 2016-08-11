#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class TaskLogic
{
    public:
        TaskLogic(const WallMap& walls, tsk::TRoomTask& tRoomTask, tsk::TWallTask& tWallTask, tsk::TDoorTask& tDoorTask);
        void update();
    private:
        void updateRoomTasks();
        void updateWallTasks();

        const WallMap& mWalls;
        tsk::TRoomTask& mTRoomTask;
        tsk::TWallTask& mTWallTask;
        tsk::TDoorTask& mTDoorTask;
};
