#pragma once
#include <cstdint>
#include <array>

enum AccessType { RandomAccess, Iteration, Addition, Deletion};

using Metrics = std::array<int64_t, 4>;
