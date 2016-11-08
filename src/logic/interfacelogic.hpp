#pragma once
#include <string>
#include <thero/optional.hpp>
#include "../glm.hpp"
#include <fea/render2d.hpp>
#include <fea/userinterface.hpp>
#include <data.hpp>
#include <fea/util.hpp>
#include <unordered_set>
#include "../wallposition.hpp"
#include "../atmosphereutil.hpp"

class Space;
class ResourceManager;

class InterfaceLogic
{
    struct RoomPlanInfo
    {
        glm::ivec2 start;
        glm::ivec2 end;
        th::Optional<glm::ivec2> initialPos;
        std::unordered_set<WallPosition> doors;
    };

    struct DoorsPlanInfo
    {
        std::unordered_set<WallPosition> doors;
        th::Optional<glm::ivec2> initialPos;
    };

    struct StructureInteraction
    {
        int32_t structureId;
        int32_t structureTypeId;
        th::Optional<glm::ivec2> initialPos;
    };

    struct BuildStructureInfo
    {
        th::Optional<int32_t> selectedStructureId;
        std::unordered_map<glm::ivec2, int32_t> plans;
    };

    struct PaintAtmosphereInfo
    {
        Gases atmosphereColor = cHealthyAtmosphere;
    };

    struct EditWallsDoorsInfo
    {
        enum PlaceState { Walls, Doors, Lock };
        enum ToggleState { On, Off };
        PlaceState placeState = Walls;
        ToggleState toggleState = On;
    };

    public:
        struct Output
        {
            std::vector<RoomTask> roomTasks;
        };

        enum State
        {
            IDLE,
            DRAGGING_ROOM, PLANNING_ROOM,
            INTERACT_STRUCTURE,
            BUILD_STRUCTURE,
            PLACING_DOORS,
            PAINT_ATMOSPHERE,
            EDIT_WALLS_DOORS,
        };

        InterfaceLogic(Space& space, fea::Renderer2D& renderer, ResourceManager& resources, int32_t& gameSpeedMultiplier, int32_t& stepAmount, bool& showZones, bool& showAtmosphere, NumberPool<int32_t>& taskIdPool, GameData& data);
        void update();
        void worldMouseClick(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseDrag(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void worldMouseRelease(const glm::ivec2& position, const glm::ivec2& tile, fea::Mouse::Button button);
        void keyPressed(int32_t code);
    private:
        std::string stateToString(State state) const;
        void reset();
        void applyEditWallsDoors(const WallPosition& position);
        State mState;

        th::Optional<glm::ivec2> mDragStart;
        th::Optional<glm::ivec2> mDragEnd;
        th::Optional<RoomPlanInfo> mRoomPlan;
        th::Optional<DoorsPlanInfo> mDoorsPlan;
        th::Optional<BuildStructureInfo> mBuildStructureInfo;
        PaintAtmosphereInfo mPaintAtmosphereInfo;
        EditWallsDoorsInfo mEditWallsDoorsInfo;

        th::Optional<StructureInteraction> mStructureInteraction;

		Space& mSpace;
        fea::Renderer2D& mRenderer;
        ResourceManager& mResources;
        int32_t& mGameSpeedMultiplier;
        int32_t& mStepAmount;
        GameData& mData;
        bool& mShowZones;
        bool& mShowAtmosphere;
        fea::FrameTimer mFrameTimer;
};
