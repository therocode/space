#include "aiutil.hpp"

std::string toString(AiType type)
{
    if(type == AiType::Human)
        return "Human";
    else
    {
        TH_ASSERT(false, "unknown ai");
        return "bleh";
    }
}
