#pragma once
#include <data.hpp>

namespace DebugGui
{
    std::vector<std::string> debugHeaders(const ent::TPosition& table);
    void debugText(const glm::vec2& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const ent::TPhysics& table);
    void debugText(const Physics& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const ent::TMoveIntention& table);
    void debugText(const MoveIntention& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const ent::TMoveAbility& table);
    void debugText(const MoveAbility& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const tsk::TRoomTask& table);
    void debugText(const RoomTask& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const tsk::TWallTask& table);
    void debugText(const WallTask& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const tsk::TDoorTask& table);
    void debugText(const DoorTask& data, std::vector<std::vector<std::string>>& outText);
    std::vector<std::string> debugHeaders(const gfx::TActorSprite& table);
    void debugText(const ActorSprite& data, std::vector<std::vector<std::string>>& outText);
}
