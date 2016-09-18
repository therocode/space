#include "incentiveutil.hpp"

std::string toString(Incentive::Type type)
{
    if(type == Incentive::Breathe)
        return "Breathe";
    else if(type == Incentive::Work)
        return "Work";
    else
    {
        TH_ASSERT(false, "unknown incentive");
        return "bleh";
    }
}
