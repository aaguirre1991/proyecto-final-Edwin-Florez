#pragma once

#include "IEmitter.h"
#include "Vec2.h"

class RainEmitter : public IEmitter
{
public:
    // areaMin = esquina superior izquierda del área de emisión
    // areaMax = esquina inferior derecha del área de emisión
    // rate    = partículas por segundo
    // baseVel = velocidad base (dirección y rapidez de la lluvia)
    RainEmitter(const Vec2& areaMin,
                const Vec2& areaMax,
                float       rate,
                const Vec2& baseVel);

    void emit(ParticleSystem& system, float dt) override;

private:
    Vec2  m_areaMin;
    Vec2  m_areaMax;
    Vec2  m_baseVel;
    float m_rate;
    float m_emitRemainder = 0.0f;   // para acumular fracciones de partículas
};
