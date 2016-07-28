#pragma once
#include "debugging.hpp"
#include "debugio.hpp"
#include "debugrenderer.hpp"
#include <data.hpp>

enum DebugFlag : int32_t { DPhysics };

std::ostream& operator<<(std::ostream& out, const IdSet& idSet);
