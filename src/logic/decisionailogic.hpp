#pragma once
#include <data.hpp>

/*
MetaTask
  - An abstract task which represents something to be done which cannot be expressed in a single task
    e.g. "Build a 4x6 sized room at 5,5 with a door at 5,6"
  - Agents cannot take on these tasks as they are abstract
  - Always consist of Tasks that make up this bigger scale task.
  - Is automatically resolved if all tasks that it consists of are solved

Task 
  - Something concrete that is to be done in the world
    e.g. "Build wall at 2,3", "Open door at 5,6" or "Search walls for leaks"
  - Agents can take on tasks to solve. They can only take on one at a time and every task can only have one worker performing it at any given time.
  - Will be resolved based on conditions about the world
    e.g. A task to build a wall at a certain position is automatically resolved when a wall exists there
  - Can be dependent on other tasks, in which case agents will have to take on the other tasks first

Action
  - Actions are specific things that agents do in the world
    e.g. "fetch material at 3,6", "assemble wall at 1,1 with carried material" or "go to 7,8".
  - Agents themselves decide on proper actions to solve Tasks and perform Orders.
    e.g. An agent takes on the task "build wall at 4,5" and starts by executing the action to "fetch material"
  - Actions can be of varying abstraction level, and have a tree-like execution structure
    e.g. To execute the action "fetch material" an agent decides to first run "localise nearest material" and then "go to <position of material>" followed by "equip space suit" and "fetch space suit" etc.
  - Actions carry state on attempted/failed solutions and whatever else data they need
  - Agents have knowledge on what actions they are capable of doing and use own judgement on which ones to utilise for any given task/order

Order
  - Agent directed instructions to perform actions in the world
    e.g. "Agent X: go to 4,5" or "Agent Y: clean up room at 5,6"
  - Orders are like tasks, solved by agents choosing actions
  - Are of more fleeting nature and will be cancelled if the given agent is not able to perform it
  - Are prioritised higher than Tasks

Incentive
  - This is what drives agents to do anything at all, with no incentive they will stand still
    e.g. incentive to work, incentive to flee from danger, incentive to eat
  - Incentives have varying degrees of importance which will decide which ones the agent will act on first. These importances change
    e.g. incentive to work gets more important during scheduled work hours. incentive to eat gradually increases with hunger
  - Incentives create actions
    e.g. Action to take on a Task to complete, Action to walk to position (to avoid danger)
  - Incentives are inherent to particular actors
    e.g. employed human workers have incentive to work, eat, sleep. Construction robots have incentive to take on construction tasks, no incentive to eat.
  - Strong change in incentive importance has the power to cancel actions belonging to other lower importance incentives
*/

class DecisionAILogic
{
    public:
        DecisionAILogic(GameData& data);
        void update();
    private:
        GameData& mData;
};
