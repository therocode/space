#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include "common.hpp"

template <typename Functor>
bool return_if_false(Functor functor)
{
    return true;
}

template <typename Functor, typename Arg, typename...Args>
bool return_if_false(Functor functor, Arg& arg, Args&...args)
{
    if(!functor(arg))
        return false;
    return return_if_false(functor, args...);
}

template <typename Arg, typename... Args>
int32_t return_id_of_collection(size_t index, Arg& arg, Args&... args)
{
    return arg.ids[index];
}

template <typename... Args>
struct InvokeHelper
{
    template <typename Functor, typename IndexArray, std::size_t... Is>
        static void invoke(const Functor& functor, const IndexArray& indices, Args&... args, std::index_sequence<Is...>)
        {
            functor(return_id_of_collection(indices[0], args...), args.data[indices[Is]]...);
        }
};

template <typename Functor, typename...Args>
void join(Functor functor, Args&...args)
{
    using swallow = int[];
    (void)swallow{(sort(args), 0)...}; //executes func on each args entry
        
    (void)swallow{(++args.meta.metrics[AccessType::Iteration], 0)...}; //executes func on each args entry

    if(!return_if_false([&](const auto& data){return !data.ids.empty();}, args...))
        return;

    auto dataCount = sizeof...(args);

    size_t indices[] = {((void)args, 0u)...};

    int32_t lowestId = std::max({args.ids[0]...});

    while(true)
    {
        bool stop = false;
        bool skip = false;

        size_t index = 0;
        auto func = [&](auto& ids)
        {
            if(skip || stop)
                return;

            while(ids[indices[index]] < lowestId)
            {
                ++indices[index];

                if(indices[index] == ids.size())
                {
                    stop = true;
                    return;
                }
            }

            if(ids[indices[index]] > lowestId)
            {
                lowestId = ids[indices[index]];
                skip = true;
                return;
            }

            ++index;
        };

        

        (void)swallow{(func(args.ids), 0)...}; //executes func on each args entry
        
        if(stop)
            return;

        if(skip)
            continue;

        InvokeHelper<Args...>::invoke(functor, indices, args..., std::make_index_sequence<sizeof...(Args)>{});

        index = 0;

        (void)swallow{((stop |= (args.ids.size() == ++indices[index++])), 0)...};

        if(stop)
            return;
    }
}
