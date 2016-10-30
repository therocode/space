#pragma once

IdSet uninitializedStructures = {{}, {"Uninitialized Structures", "Structures that need initialization"}};
IdSet openDoors = {{}, {"Open doors", "All doors that are open"}};
IdSet lockedDoors = {{}, {"Locked doors", "All doors that are locked"}};
IdSet unassignedTasks = {{}, {"Unassigned tasks", "All tasks which no worker is currently on"}};
IdSet died = {{}, {"Died", "The organisms that have just died, will only be set for one frame"}};
IdSet leafActions = {{}, {"Leaf actions", "Actions that have no children."}};
IdSet activeIncentives = {{}, {"Active Incentives", "The incentives that are the ones currently acted upon"}};
IdSet uninitializedAis = {{}, {"Uninitialized AIs", "AI agents that are in need of initialization"}};
IdSet humanAis = {{}, {"Human AIs", "AI agents that think like a human"}};
IdSet builders = {{}, {"Builders", "All workers who are builders"}};
IdSet freeWorkers = {{}, {"Free workers", "All workers who are currently not working on any task"}};
IdSet deadWorkers = {{}, {"Dead workers", "All workers who are no longer alive"}};

WallMap walls;
WallChanges wallChanges;
Zones zones;
Grid<Gases> atmosphere;
Grid<GridNeighbors<Gases>> atmosphereNeighbors;
