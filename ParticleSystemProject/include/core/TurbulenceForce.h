#pragma once

#include "IForce.h"
#include "Vec2.h"

class TurbulenceForce : public IForce {
public:
    // strength = intensidad de la turbulencia (más grande = más caótico)
    explicit TurbulenceForce(float strength);

    void apply(Particle& p, float dt) override;

private:
    float strength;
};
