#pragma once
#include <type_traits>
#include <set>
#include <thero/assert.hpp>

template <typename IntegralType>
class NumberPool
{
    public:
        NumberPool():
            mNext(0)
        {
            static_assert(std::is_integral<IntegralType>::value, "Only supports integral types");
        }

        IntegralType next()
        {
            auto iterator = mReturned.begin();

            if(iterator != mReturned.end())
            {
                IntegralType number = *iterator;
                mReturned.erase(iterator);

                return number;
            }
            else
            {
                return mNext++;
            }
        }

        void release(IntegralType value)
        {
            TH_ASSERT(value < mNext, "Returning value " + std::to_string(value) + " which has never been given out");
            TH_ASSERT(mReturned.count(value) == 0, "Returning value " + std::to_string(value) + " which has already been returned");
            mReturned.emplace(value);
        }
    private:
        IntegralType mNext;
        std::set<IntegralType> mReturned;
};
