#pragma once
#include <cstdint>
#include <array>

enum AccessType { RandomAccess, Iteration, Addition, Deletion, Sort};

using Metrics = std::array<int64_t, 5>;
