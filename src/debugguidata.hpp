#pragma once
#include <data.hpp>

namespace DebugGui
{
    std::vector<std::string> debugHeaders(const TPosition& table);
    void debugText(const glm::vec2& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TPhysics& table);
    void debugText(const Physics& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TCollisionBox& table);
    void debugText(const CollisionBox& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TMoveIntention& table);
    void debugText(const MoveIntention& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TMoveAbility& table);
    void debugText(const MoveAbility& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TBloodValues& table);
    void debugText(const BloodValues& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TChoking& table);
    void debugText(const Choking& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TStructure& table);
    void debugText(const Structure& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TStructureType& table);
    void debugText(const StructureType& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TBusyWorker& table);
    void debugText(const BusyWorker& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TRoomTask& table);
    void debugText(const RoomTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TWallTask& table);
    void debugText(const WallTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TDoorTask& table);
    void debugText(const DoorTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TAssignedTask& table);
    void debugText(const AssignedTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TActorSprite& table);
    void debugText(const ActorSprite& data, std::vector<std::vector<std::string>>& outText);
}
