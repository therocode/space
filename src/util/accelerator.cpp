#include "accelerator.hpp"

glm::vec2 Accelerator::get(glm::vec2 direction, float maxSpeed, const glm::vec2& currentVelocity, float maxAcc)
{
    if(glm::length(direction) > 0.0f)
        direction = glm::normalize(direction);

    glm::vec2 targetVel = direction * maxSpeed;
    glm::vec2 currentVel = currentVelocity;

    glm::vec2 acc = (targetVel - currentVel) / 1.0f;  //  / timestep

    if(acc.x > maxAcc)
        acc.x = maxAcc;
    if(acc.x < -maxAcc)
        acc.x = -maxAcc;
    if(acc.y > maxAcc)
        acc.y = maxAcc;
    if(acc.y < -maxAcc)
        acc.y = -maxAcc;

    return acc;
}
