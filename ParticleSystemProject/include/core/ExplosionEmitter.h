#pragma once

#include "IEmitter.h"
#include "Vec2.h"

class ExplosionEmitter : public IEmitter
{
public:
    ExplosionEmitter(const Vec2& origin,
                     float       minSpeed,
                     float       maxSpeed,
                     float       minLife,
                     float       maxLife,
                     int         count);

    // Atajo: velocidad base + cantidad
    ExplosionEmitter(const Vec2& origin,
                     float       baseSpeed,
                     int         count);

    void setOrigin(const Vec2& origin) { m_origin = origin; }

    // Marca que queremos detonar en el próximo update
    void trigger();

    // Implementa IEmitter
    void emit(ParticleSystem& system, float dt) override;

private:
    Vec2  m_origin;
    float m_minSpeed;
    float m_maxSpeed;
    float m_minLife;
    float m_maxLife;
    int   m_count;

    bool  m_pending = false;   // ¿hay explosión pendiente?
};
