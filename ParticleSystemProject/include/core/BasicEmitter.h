#pragma once

#include "IEmitter.h"
#include "Vec2.h"
#include <random>

class BasicEmitter : public IEmitter {
public:
    BasicEmitter(Vec2 position, float rate);

    void emit(ParticleSystem& system, float dt) override;

private:
    Vec2 position;      // posición base del emisor
    float rate;         // partículas por segundo
    float accumulator;  // para emitir según dt
};
