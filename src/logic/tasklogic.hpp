#pragma once
#include "data.hpp"
#include "../wallmap.hpp"

class TaskLogic
{
    public:
        TaskLogic(GameData& data);
        void update();
    private:
        void updateRoomTasks();
        void updateWallTasks();
        void updateDoorTasks();
        void updateStructureTasks();

        GameData& mData;
};
