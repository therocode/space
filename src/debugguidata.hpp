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

    std::vector<std::string> debugHeaders(const TAi& table);
    void debugText(const Ai& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TIncentive& table);
    void debugText(const Incentive& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TBreatheIncentive& table);
    void debugText(const BreatheIncentive& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TWorkIncentive& table);
    void debugText(const WorkIncentive& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TAction& table);
    void debugText(const Action& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TTaskAction& table);
    void debugText(const TaskAction& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TGotoAction& table);
    void debugText(const GotoAction& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TTotalPanicAction& table);
    void debugText(const TotalPanicAction& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TFindWorkTaskAction& table);
    void debugText(const FindWorkTaskAction& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TConstructWallAction& table);
    void debugText(const ConstructWallAction& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TConstructDoorAction& table);
    void debugText(const ConstructDoorAction& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TStructure& table);
    void debugText(const Structure& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TStructureType& table);
    void debugText(const StructureType& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TAirlock& table);
    void debugText(const Airlock& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TAirlockActivity& table);
    void debugText(const AirlockActivity& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TDoor& table);
    void debugText(const Door& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TStructureDoorLock& table);
    void debugText(const StructureDoorLock& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TZoneLeak& table);
    void debugText(const ZoneLeak& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TBusyWorker& table);
    void debugText(const BusyWorker& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TPath& table);
    void debugText(const Path& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TTask& table);
    void debugText(const Task& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TRoomTask& table);
    void debugText(const RoomTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TWallTask& table);
    void debugText(const WallTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TDoorTask& table);
    void debugText(const DoorTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TAssignedTask& table);
    void debugText(const AssignedTask& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TItem& table);
    void debugText(const Item& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TWearable& table);
    void debugText(const Wearable& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TItemContainer& table);
    void debugText(const ItemContainer& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TItemStoring& table);
    void debugText(const ItemStoring& data, std::vector<std::vector<std::string>>& outText);

    std::vector<std::string> debugHeaders(const TActorSprite& table);
    void debugText(const ActorSprite& data, std::vector<std::vector<std::string>>& outText);
}
