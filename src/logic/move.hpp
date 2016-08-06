#pragma once
#include "data.hpp"

void calculateMoveIntention(const ent::TWalkTarget& tWalkTarget, ent::TMoveIntention& tMoveIntention, const ent::TPosition& tPosition);
void applyMoveIntention(const ent::TMoveIntention& tMoveIntention, const ent::TMoveAbility& tMoveAbility, ent::TPhysics& tPhysics);
