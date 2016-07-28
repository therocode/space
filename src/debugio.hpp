#pragma once
#include <ostream>
#include <vector>
#include "data.hpp"

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vector)
{
    os << "s: " << vector.size() << " [";
    for(const auto& entry : vector)
        os << entry << ", ";
    os << "]";

    return os;
}
