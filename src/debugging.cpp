#include "debugging.hpp"

namespace dbg
{
    std::unordered_map<std::string, th::Any> dvars = {};

    bool exists(const std::string& name)
    {
        return dvars.count(name) != 0;
    }

    void erase(const std::string& name)
    {
        dvars.erase(name);
    }

    bool getFlag(DebugFlag flag)
    {
        auto iter = debugFlags.find(flag);

        if(iter != debugFlags.end())
            return iter->second;

        return false;
    }

    bool flipFlag(DebugFlag flag)
    {
        auto iter = debugFlags.find(flag);

        if(iter != debugFlags.end())
        {
            return iter->second = !iter->second;
        }
        else
        {
            return debugFlags[flag] = true;
        }
    }

    bool setFlag(DebugFlag flag, bool state)
    {
        return debugFlags[flag] = state;
    }

    std::unordered_map<DebugFlag, bool> debugFlags = {};
}
