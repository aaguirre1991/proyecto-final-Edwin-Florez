#include "../include/core/RainEmitter.h"
#include "../include/core/ParticleSystem.h"

#include <cmath>
#include <cstdlib>

// Funciones helper similares a las que ya usas
namespace
{
    float randRange(float min, float max)
    {
        return min + (max - min) * (std::rand() / static_cast<float>(RAND_MAX));
    }

    float length(const Vec2& v)
    {
        return std::sqrt(v.x * v.x + v.y * v.y);
    }

    Vec2 normalize(const Vec2& v)
    {
        float len = length(v);
        if (len < 1e-6f) return {0.0f, 0.0f};
        return { v.x / len, v.y / len };
    }
}

// ------------------------------------------------------------
// Constructor
// ------------------------------------------------------------
RainEmitter::RainEmitter(const Vec2& areaMin,
                         const Vec2& areaMax,
                         float       rate,
                         const Vec2& baseVel)
    : m_areaMin(areaMin)
    , m_areaMax(areaMax)
    , m_baseVel(baseVel)
    , m_rate(rate)
{
}

// ------------------------------------------------------------
// Emitir partículas estilo "fuente"
// ------------------------------------------------------------
void RainEmitter::emit(ParticleSystem& system, float dt)
{
    if (m_rate <= 0.0f)
        return;

    // Cuántas partículas tocaría emitir este frame (con acumulación de fracción)
    float particlesToEmit = m_rate * dt + m_emitRemainder;
    int   count           = static_cast<int>(particlesToEmit);
    m_emitRemainder       = particlesToEmit - static_cast<float>(count);

    if (count <= 0)
        return;

    Vec2 baseDir   = normalize(m_baseVel);
    float baseSpeed = length(m_baseVel);
    if (baseSpeed < 1e-4f)
        return;

    float baseAngle = std::atan2(baseDir.y, baseDir.x);

    // Parámetros del efecto fuente
    const float jetSpreadAngle   = 0.20f;  // núcleo casi recto
    const float spraySpreadAngle = 1.10f;  // spray lateral (~63°)
    const float jetRatio         = 0.6f;   // 60% partículas del chorro central

    for (int i = 0; i < count; ++i)
    {
        Particle p;

        // Posición dentro del área de emisión (boca de la fuente)
        float x = randRange(m_areaMin.x, m_areaMax.x);
        float y = randRange(m_areaMin.y, m_areaMax.y);
        p.position = { x, y };

        // ¿Esta partícula es del chorro central o del spray?
        float kind = randRange(0.0f, 1.0f);

        float angleOffset;
        float speedFactor;
        float lifeMin, lifeMax;

        if (kind < jetRatio)
        {
            // Núcleo: casi vertical, rápido y con vida un poco más larga
            angleOffset = randRange(-jetSpreadAngle, jetSpreadAngle);
            speedFactor = randRange(0.9f, 1.1f);
            lifeMin     = 1.1f;
            lifeMax     = 1.6f;
        }
        else
        {
            // Spray: más abierto, un poco más lento y vida más corta
            angleOffset = randRange(-spraySpreadAngle, spraySpreadAngle);
            speedFactor = randRange(0.6f, 0.9f);
            lifeMin     = 0.7f;
            lifeMax     = 1.2f;
        }

        float angle = baseAngle + angleOffset;
        float speed = baseSpeed * speedFactor;

        p.velocity.x = speed * std::cos(angle);
        p.velocity.y = speed * std::sin(angle);

        p.maxLife = randRange(lifeMin, lifeMax);
        p.life    = p.maxLife;

        // IMPORTANTE: usar la misma API que FireEmitter
        system.emit(p);
    }
}
