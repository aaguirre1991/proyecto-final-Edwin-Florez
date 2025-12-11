#pragma once
#include "IEmitter.h"
#include "Vec2.h"
#include "Particle.h"
#include <algorithm> // std::clamp

class FireEmitter : public IEmitter
{
public:
    FireEmitter(Vec2 position, float baseRate);

    void emit(ParticleSystem& system, float dt) override;

    // ---- NUEVO: controlar intensidad externa ----
    void setIntensity(float value)
    {
        intensity = std::clamp(value, 0.0f, 5.0f); // 0 = apagado, 5 = muy intenso
    }

    float getIntensity() const { return intensity; }

private:
    Vec2  position;
    float baseRate;      // partículas por segundo a intensidad = 1
    float accumulator{}; // para emitir número entero de partículas
    float intensity{1.0f};
};
