#include "orientation.hpp"

std::string to_string(const Orientation& orientation)
{
    if(orientation == Orientation::Vertical)
        return "vertical";
    else
        return "horizontal";
}
