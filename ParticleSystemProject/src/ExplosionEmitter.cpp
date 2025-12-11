#include "../include/core/ExplosionEmitter.h"
#include "../include/core/ParticleSystem.h"

#include <cstdlib>
#include <cmath>

namespace
{
    float rand01()
    {
        return static_cast<float>(std::rand()) /
               static_cast<float>(RAND_MAX);
    }
}

ExplosionEmitter::ExplosionEmitter(const Vec2& origin,
                                   float       minSpeed,
                                   float       maxSpeed,
                                   float       minLife,
                                   float       maxLife,
                                   int         count)
: m_origin(origin)
, m_minSpeed(minSpeed)
, m_maxSpeed(maxSpeed)
, m_minLife(minLife)
, m_maxLife(maxLife)
, m_count(count)
{
}

ExplosionEmitter::ExplosionEmitter(const Vec2& origin,
                                   float       baseSpeed,
                                   int         count)
: ExplosionEmitter(origin,
                   baseSpeed * 0.7f,
                   baseSpeed * 1.3f,
                   0.4f,
                   1.2f,
                   count)
{
}

void ExplosionEmitter::trigger()
{
    m_pending = true; // la detonación se hará en el próximo emit()
}

void ExplosionEmitter::emit(ParticleSystem& system, float /*dt*/)
{
    if (!m_pending)
        return;

    m_pending = false; // consumimos la explosión pendiente

    const float twoPi = 6.28318530718f;

    for (int i = 0; i < m_count; ++i)
    {
        float angle = rand01() * twoPi;
        float speed = m_minSpeed + (m_maxSpeed - m_minSpeed) * rand01();
        float life  = m_minLife + (m_maxLife - m_minLife) * rand01();

        Vec2 vel{ std::cos(angle) * speed,
                  std::sin(angle) * speed };

        Particle p;
        p.position = m_origin;
        p.velocity = vel;
        p.life     = life;
        p.maxLife  = life;

        system.emit(p);   // mismo estilo que FireEmitter y RainEmitter
    }
}