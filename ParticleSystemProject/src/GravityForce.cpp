#include "../include/core/GravityForce.h"
#include "../include/core/Particle.h"

GravityForce::GravityForce(const Vec2& acceleration)
    : m_acceleration(acceleration)
{
}

void GravityForce::apply(Particle& p, float dt)
{
    // v = v + a * dt
    p.velocity = p.velocity + m_acceleration * dt;
}
