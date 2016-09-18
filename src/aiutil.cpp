#include "aiutil.hpp"

std::string toString(Ai::Type type)
{
    if(type == Ai::Human)
        return "Human";
    else
    {
        TH_ASSERT(false, "unknown ai");
        return "bleh";
    }
}
