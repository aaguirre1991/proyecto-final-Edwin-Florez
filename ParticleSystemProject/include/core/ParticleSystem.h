#pragma once

#include <vector>
#include <cmath>
#include "Particle.h"
#include "IEmitter.h"
#include "IForce.h"
#include "Vec2.h"

class ParticleSystem
{
public:
    explicit ParticleSystem(size_t maxParticles);

    // Actualiza todas las partículas
    void update(float dt);

    // Crear una partícula
    void emit(const Particle& p);

    // Emitters
    void addEmitter(IEmitter* emitter) { emitters.push_back(emitter); }

    // Forces
    void addForce(IForce* force) { forces.push_back(force); }

    // SISTEMA DE SUELO (YA EXISTENTE)
    void enableGround(float y, float restitution = 0.3f)
    {
        m_groundEnabled     = true;
        m_groundY           = y;
        m_groundRestitution = restitution;
    }

    void disableGround() { m_groundEnabled = false; }

    // Acceso a partículas
    const std::vector<Particle>& getParticles() const { return particles; }

    // ==========================
    // 🔥 NUEVO: INTERACCIÓN ENTRE PARTÍCULAS
    // ==========================
    void setInteractionsEnabled(bool enabled) { m_enableInteractions = enabled; }
    void setInteractionRadius(float r)        { m_interRadius = r; }
    void setInteractionStrength(float k)      { m_interStrength = k; }

private:
    std::vector<Particle> particles;
    std::vector<IEmitter*> emitters;
    std::vector<IForce*>   forces;
    size_t maxCount;

    // ---- Suelo ----
    bool  m_groundEnabled     = false;
    float m_groundY           = 0.0f;
    float m_groundRestitution = 0.3f;

    // ---- Interacciones entre partículas ----
    bool  m_enableInteractions = false;
    float m_interRadius        = 30.f;   // distancia máxima
    float m_interStrength      = 150.f;  // fuerza de repulsión

    // Función privada nueva
    void applyParticleInteractions(float dt);
};
