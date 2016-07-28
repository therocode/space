#include "debug.hpp"

std::ostream& operator<<(std::ostream& out, const IdSet& idSet)
{
    out << "size: " << idSet.ids.size() << " content: \n";
    for(int32_t id : idSet.ids)
        out << id << " ";
    out << "\n";
    return out;
}
