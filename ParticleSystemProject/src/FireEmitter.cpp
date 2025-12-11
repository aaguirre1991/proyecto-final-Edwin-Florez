#include "../include/core/FireEmitter.h"
#include "../include/core/ParticleSystem.h"
#include <cstdlib>
#include <cmath>


FireEmitter::FireEmitter(Vec2 pos, float baseRate_)
    : position(pos)
    , baseRate(baseRate_)
{
}

void FireEmitter::emit(ParticleSystem& system, float dt)
{
    // cuantas partículas queremos este frame
    float rate = baseRate * intensity;
    accumulator += rate * dt;

    while (accumulator >= 1.0f)
    {
        accumulator -= 1.0f;

        Particle p;
        p.position = position;

        // velocidad hacia arriba con algo de dispersión
        float angle = -90.0f + (std::rand() % 40 - 20); // grados
        float speed = 80.0f + std::rand() % 120;        // 80–200

        float rad = angle * 3.14159265f / 180.0f;
        p.velocity = Vec2(std::cos(rad), std::sin(rad)) * speed;

        p.maxLife = 1.2f;
        p.life    = p.maxLife;

        system.emit(p);
    }
}
