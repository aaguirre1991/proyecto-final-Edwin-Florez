#pragma once

#include "Particle.h"

class IForce {
public:
    virtual ~IForce() = default;

    // Aplica la fuerza a una partícula durante dt segundos
    virtual void apply(Particle& p, float dt) = 0;
};
