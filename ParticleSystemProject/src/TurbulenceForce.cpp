#include "../include/core/TurbulenceForce.h"
#include "../include/core/ParticleSystem.h"
#include "../include/core/Particle.h"
#include <cstdlib>
#include <cmath>

// número aleatorio en [-1, 1]
static float randomSigned()
{
    return 2.0f * (static_cast<float>(rand()) / static_cast<float>(RAND_MAX)) - 1.0f;
}

TurbulenceForce::TurbulenceForce(float strength)
    : strength(strength)
{
}

void TurbulenceForce::apply(Particle& p, float dt)
{
    // Pequeña aceleración aleatoria, más fuerte en X que en Y
    Vec2 a;
    a.x = randomSigned() * strength;         // viento lateral
    a.y = randomSigned() * 0.5f * strength;  // variación ligera en Y

    p.velocity = p.velocity + a * dt;
}
