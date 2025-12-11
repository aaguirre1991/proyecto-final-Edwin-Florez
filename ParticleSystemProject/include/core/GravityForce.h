#pragma once

#include "IForce.h"
#include "Vec2.h"

// Fuerza de "gravedad" (o flotabilidad si el vector apunta hacia arriba)
class GravityForce : public IForce
{
public:
    // acceleration: vector de aceleración (por ejemplo {0, 100} o {0, -50})
    explicit GravityForce(const Vec2& acceleration);

    // Aplica la fuerza a una partícula
    void apply(Particle& p, float dt) override;

private:
    Vec2 m_acceleration;
};
