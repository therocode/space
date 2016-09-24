#include "debugguidata.hpp"
#include "taskutil.hpp"
#include "incentiveutil.hpp"
#include "aiutil.hpp"
#include "actionutil.hpp"
#include <sstream>

namespace DebugGui
{
    template<typename Vec2>
    std::vector<std::string> vec2ToStringList(const Vec2& vec2)
    {
        return
        {
            std::to_string(vec2.x) + " " + std::to_string(vec2.y),
                "x: " + std::to_string(vec2.x),
                "y: " + std::to_string(vec2.y),
        };
    }

    std::vector<std::string> colorToStringList(const fea::Color& color)
    {
        return
        {
            std::to_string(+color.r) + " " + std::to_string(+color.g) + " " + std::to_string(+color.b) + " " + std::to_string(+color.a),
                "r: " + std::to_string(+color.r),
                "g: " + std::to_string(+color.g),
                "b: " + std::to_string(+color.b),
                "a: " + std::to_string(+color.a),
        };
    }

    template<typename T>
    std::vector<std::string> vectorToStringList(const std::vector<T>& list)
    {
        std::vector<std::string> result;

        std::stringstream total;

        for(const auto& item : list)
        {
            std::stringstream ss;
            ss << item;
            total << item << ", ";
            result.push_back(ss.str());
        }

        std::string totalString = total.str();

        if(!list.empty())
        {
            totalString.pop_back();
            totalString.pop_back();
        }

        result.insert(result.begin(), totalString);
        return result;
    }

    std::vector<std::string> airlockModeToStringList(Airlock::Mode mode)
    {
        if(mode == Airlock::In)
            return {"In"};
        else if(mode == Airlock::Out)
            return {"Out"};
        else if(mode == Airlock::Pumping)
            return {"Pumping"};
        return {"Unknown"};
    }

    std::vector<std::string> debugHeaders(const TPosition& table)
    {
        return {"Position"};
    }

    void debugText(const glm::vec2& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            vec2ToStringList(data),
        };
    }

    std::vector<std::string> debugHeaders(const TPhysics& table)
    {
        return {"Acceleration", "Velocity"};
    }

    void debugText(const Physics& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            vec2ToStringList(data.acceleration),
            vec2ToStringList(data.velocity),
        };
    }

    std::vector<std::string> debugHeaders(const TCollisionBox& table)
    {
        return {"Size",};
    }

    void debugText(const CollisionBox& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            vec2ToStringList(data.size),
        };
    }

    std::vector<std::string> debugHeaders(const TMoveIntention& table)
    {
        return {"Direction", "Speed Percent"};
    }

    void debugText(const MoveIntention& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            vec2ToStringList(data.direction),
            {
                std::to_string(data.speedPercent * 100) + "%",
            },
        };
    }

    std::vector<std::string> debugHeaders(const TMoveAbility& table)
    {
        return {"Max Speed", "Max Acceleration"};
    }

    void debugText(const MoveAbility& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.maxSpeed),
            },
            {
                std::to_string(data.maxAcceleration),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TBloodValues& table)
    {
        return {"Oxygen"};
    }

    void debugText(const BloodValues& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.oxygen),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TChoking& table)
    {
        return {"Choking"};
    }

    void debugText(const Choking& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.breathingCapability * 100) + "%",
            },
        };
    }

    std::vector<std::string> debugHeaders(const TAi& table)
    {
        return {"Type"};
    }

    void debugText(const Ai& data, std::vector<std::vector<std::string>>& outText)
    {
        outText = 
        {
            {
                toString(data.type),
            }
        };
    }


    std::vector<std::string> debugHeaders(const TIncentive& table)
    {
        return {"Actor ID", "Importance", "Type"};
    }

    void debugText(const Incentive& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.actorId),
            },
            {
                std::to_string(data.importance),
            },
            {
                toString(data.type),
            },
        };
    }


    std::vector<std::string> debugHeaders(const TBreatheIncentive& table)
    {
        return {""};
    }

    void debugText(const BreatheIncentive& data, std::vector<std::vector<std::string>>& outText)
    {
        outText = 
        {
            {""},
        };
    }


    std::vector<std::string> debugHeaders(const TWorkIncentive& table)
    {
        return {""};
    }

    void debugText(const WorkIncentive& data, std::vector<std::vector<std::string>>& outText)
    {
        outText = 
        {
            {""},
        };
    }

    std::vector<std::string> debugHeaders(const TAction& table)
    {
        return {"Actor ID", "Parent action", "Action type"};
    }

    void debugText(const Action& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.actorId),
            },
            {
                data.parentAction ? std::to_string(*data.parentAction) : std::string("None"),
            },
            {
                {toString(data.type)},
            },
        };
    }

    std::vector<std::string> debugHeaders(const TTaskAction& table)
    {
        return {"Action ID", "Task ID",};
    }

    void debugText(const TaskAction& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.actionId),
            },
            {
                std::to_string(data.taskId),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TGotoAction& table)
    {
        return {"Target", "Acceptable Distance", "PathId"};
    }

    void debugText(const GotoAction& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.target),
            },
            {
                std::to_string(data.acceptableDistance),
            },
            {
                data.pathId ? std::to_string(*data.pathId) : std::string("None"),
            },
        };
    }


    std::vector<std::string> debugHeaders(const TTotalPanicAction& table)
    {
        return {"", };
    }

    void debugText(const TotalPanicAction& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                ""
            },
        };
    }

    std::vector<std::string> debugHeaders(const TFindWorkTaskAction& table)
    {
        return {"", };
    }

    void debugText(const FindWorkTaskAction& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                ""
            },
        };
    }

    std::vector<std::string> debugHeaders(const TConstructWallAction& table)
    {
        return {"Work Left"};
    }

    void debugText(const ConstructWallAction& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.workLeft),
            }
        };
    }

    std::vector<std::string> debugHeaders(const TConstructDoorAction& table)
    {
        return {"Work Left"};
    }

    void debugText(const ConstructDoorAction& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.workLeft),
            }
        };
    }

    std::vector<std::string> debugHeaders(const TStructure& table)
    {
        return {"Type", "Position"};
    }

    void debugText(const Structure& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.structureType),
            },
            {
                vec2ToStringList(data.position),
            }
        };
    }

    std::vector<std::string> debugHeaders(const TAirlock& table)
    {
        return {"Doors", "Exit"};
    }

    void debugText(const Airlock& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                airlockModeToStringList(data.currentMode),
            },
            {
                vectorToStringList(data.doors),
            },
            {
                data.exit ? std::to_string(*data.exit) : "none",
            },
        };
    }

    std::vector<std::string> debugHeaders(const TAirlockActivity& table)
    {
        return {"Target Mode", "Currently Pumping", "Leak Id"};
    }

    void debugText(const AirlockActivity& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                airlockModeToStringList(data.targetMode),
            },
            {
                airlockModeToStringList(data.currentlyPumping),
            },
            {
                std::to_string(data.leakId),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TDoor& table)
    {
        return {"Position", "Orientation"};
    }

    void debugText(const Door& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.position.position),
            },
            {
                {to_string(data.position.orientation)},
            },
        };
    }

    std::vector<std::string> debugHeaders(const TStructureDoorLock& table)
    {
        return {"Structure ID", "Door ID"};
    }

    void debugText(const StructureDoorLock& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                {std::to_string(data.structureId)},
            },
            {
                {std::to_string(data.doorId)},
            },
        };
    }

    std::vector<std::string> debugHeaders(const TZoneLeak& table)
    {
        return {"Start Position", "End Position", "Hole Size", "Pump Force"};
    }

    void debugText(const ZoneLeak& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.start),
            },
            {
                vec2ToStringList(data.end),
            },
            {
                {std::to_string(data.size)},
            },
            {
                {std::to_string(data.pumpForce)},
            },
        };
    }

    std::vector<std::string> debugHeaders(const TStructureType& table)
    {
        return {"Name"};
    }

    void debugText(const StructureType& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                data.name
            },
        };
    }

    std::vector<std::string> debugHeaders(const TBusyWorker& table)
    {
        return {"Task ID",};
    }

    void debugText(const BusyWorker& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.taskId),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TTask& table)
    {
        return {"Priority", "Type"};
    }

    void debugText(const Task& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.priority),
            },
            {
                toString(data.type),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TRoomTask& table)
    {
        return {"Position", "Size"};
    }

    void debugText(const RoomTask& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.position),
            },
            {
                vec2ToStringList(data.size),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TWallTask& table)
    {
        return {"Position", "Orientation"};
    }

    void debugText(const WallTask& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.position.position),
            },
            {
                to_string(data.position.orientation),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TDoorTask& table)
    {
        return {"Position", "Orientation"};
    }

    void debugText(const DoorTask& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.position.position),
            },
            {
                to_string(data.position.orientation),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TAssignedTask& table)
    {
        return {"Assignee ID"};
    }

    void debugText(const AssignedTask& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.assigneeId),
            },
        };
    }

    std::vector<std::string> debugHeaders(const TActorSprite& table)
    {
        return {"Actor ID", "Texture ID", "Color"};
    }

    void debugText(const ActorSprite& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                std::to_string(data.actorId),
            },
            {
                std::to_string(data.textureId),
            },
            colorToStringList(data.color),
        };
    }
}
