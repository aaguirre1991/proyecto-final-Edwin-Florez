#include "../include/core/BasicEmitter.h"
#include "../include/core/ParticleSystem.h"
#include "../include/core/Particle.h"
#include "../include/core/Vec2.h"

BasicEmitter::BasicEmitter(Vec2 position, float rate)
    : position(position), rate(rate), accumulator(0.0f)
{
}

void BasicEmitter::emit(ParticleSystem& system, float dt)
{
    accumulator += rate * dt;

    while (accumulator >= 1.0f) {
        Particle p;
        p.position = position;
        p.velocity = Vec2{
            (float)(rand() % 100 - 50) / 100.0f,   // velocidad aleatoria X
            (float)(rand() % 100) / 100.0f + 1.0f  // Y positiva
        };
        p.life = 1.0f;
        p.maxLife = 1.0f;

        system.emit(p);
        accumulator -= 1.0f;
    }
}
