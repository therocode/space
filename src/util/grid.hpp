#pragma once
#include <cstdint>
#include <vector>
#include <thero/assert.hpp>
#include "../glm.hpp"

template<typename Type>
class Grid
{
    public:
        Grid() = default;

        Grid(const glm::ivec2& size, const Type& initialValue):
            mSize(size),
            mCells(static_cast<size_t>(size.x * size.y), initialValue)
        {
        }

        const Type& at(const glm::ivec2& position) const
        {
            TH_ASSERT(toIndex(position) < mCells.size(), "index '" + glm::to_string(position) + "' out of bounds");
            return mCells[toIndex(position)];
        }

        Type& at(const glm::ivec2& position)
        {
            TH_ASSERT(toIndex(position) < mCells.size(), "index '" + glm::to_string(position) + "' out of bounds");
            return mCells[toIndex(position)];
        }

        const Type& at(size_t index) const
        {
            TH_ASSERT(index < mCells.size(), "index '" + std::to_string(index) + "' out of bounds");
            return mCells[index];
        }

        Type& at(size_t index)
        {
            TH_ASSERT(index < mCells.size(), "index '" + std::to_string(index) + "' out of bounds");
            return mCells[index];
        }

        void set(const glm::ivec2& position, Type value)
        {
            TH_ASSERT(toIndex(position) < mCells.size(), "index '" + glm::to_string(position) + "' out of bounds");
            mCells[toIndex(position)] = std::move(value);
        }

        void fill(const Type& value)
        {
            std::fill(mCells.begin(), mCells.end(), value);
        }

        const glm::ivec2& size() const
        {
            return mSize;
        }

        bool inBounds(const glm::ivec2& position) const
        {
            return position.x >= 0 && position.y >= 0 && position.x < mSize.x && position.y < mSize.y;
        }
    private:
        size_t toIndex(const glm::ivec2& position) const
        {
            return static_cast<size_t>(position.x + position.y * mSize.x);
        }

        glm::ivec2 mSize;
        std::vector<Type> mCells;
};
