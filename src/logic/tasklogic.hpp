#pragma once
#include "data.hpp"
#include "../util/numberpool.hpp"
#include "../wallmap.hpp"

class TaskLogic
{
    public:
        TaskLogic(GameData& data, const WallMap& walls);
        void update();
    private:
        void updateRoomTasks();
        void updateWallTasks();
        void updateDoorTasks();

        GameData& mData;
        const WallMap& mWalls;
};
