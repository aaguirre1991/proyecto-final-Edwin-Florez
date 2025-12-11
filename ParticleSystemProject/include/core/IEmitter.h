#pragma once

#include "Particle.h"

class ParticleSystem;

class IEmitter {
public:
    virtual ~IEmitter() = default;

    // dt = tiempo transcurrido en segundos
    // el emitter debe crear partículas dentro del ParticleSystem
    virtual void emit(ParticleSystem& system, float dt) = 0;
};
