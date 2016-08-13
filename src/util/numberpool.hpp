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
            TH_ASSERT(value < mNext && mReturned.count(value) == 0, "Returning invalid value " + std::to_string(value));
            mReturned.emplace(value);
        }
    private:
        IntegralType mNext;
        std::set<IntegralType> mReturned;
};
