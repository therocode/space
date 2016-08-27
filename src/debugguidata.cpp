#include "debugguidata.hpp"

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

    std::vector<std::string> debugHeaders(const ent::TPosition& table)
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

    std::vector<std::string> debugHeaders(const ent::TPhysics& table)
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

    std::vector<std::string> debugHeaders(const ent::TCollisionBox& table)
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

    std::vector<std::string> debugHeaders(const ent::TMoveIntention& table)
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

    std::vector<std::string> debugHeaders(const ent::TMoveAbility& table)
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

    std::vector<std::string> debugHeaders(const ent::TBloodValues& table)
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

    std::vector<std::string> debugHeaders(const ent::TChoking& table)
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

    std::vector<std::string> debugHeaders(const str::TStructure& table)
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

    std::vector<std::string> debugHeaders(const str::TStructureType& table)
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

    std::vector<std::string> debugHeaders(const ent::TBusyWorker& table)
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

    std::vector<std::string> debugHeaders(const tsk::TRoomTask& table)
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

    std::vector<std::string> debugHeaders(const tsk::TWallTask& table)
    {
        return {"Position", "Orientation"};
    }

    void debugText(const WallTask& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.position),
            },
            {
                to_string(data.orientation),
            },
        };
    }

    std::vector<std::string> debugHeaders(const tsk::TDoorTask& table)
    {
        return {"Position", "Orientation"};
    }

    void debugText(const DoorTask& data, std::vector<std::vector<std::string>>& outText)
    {
        outText =
        {
            {
                vec2ToStringList(data.position),
            },
            {
                to_string(data.orientation),
            },
        };
    }

    std::vector<std::string> debugHeaders(const tsk::TAssignedTask& table)
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

    std::vector<std::string> debugHeaders(const gfx::TActorSprite& table)
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
