#include "incentiveutil.hpp"

std::string toString(Incentive::Type type)
{
    if(type == Incentive::BreatheIncentive)
        return "Breathe";
    else if(type == Incentive::WorkIncentive)
        return "Work";
    else
    {
        TH_ASSERT(false, "unknown incentive");
        return "bleh";
    }
}
