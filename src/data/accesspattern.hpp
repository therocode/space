#pragma once
#include <cstdint>
#include <numeric>
#include "metrics.hpp"

struct AccessPatternInfo
{
    std::array<float, 5> percent;
    std::array<int64_t, 5> total;
};

template <typename Container>
AccessPatternInfo accessPatternInfo(const Container& container)
{
    AccessPatternInfo info;

    info.total = container.meta.metrics;

    float total = static_cast<float>(std::accumulate(info.total.begin(), info.total.end(), 0));

    info.percent[AccessType::RandomAccess] = info.total[AccessType::RandomAccess] / total;
    info.percent[AccessType::Iteration] = info.total[AccessType::Iteration] / total;
    info.percent[AccessType::Addition] = info.total[AccessType::Addition] / total;
    info.percent[AccessType::Deletion] = info.total[AccessType::Deletion] / total;
    info.percent[AccessType::Sort] = info.total[AccessType::Sort] / total;

    return info;
}
