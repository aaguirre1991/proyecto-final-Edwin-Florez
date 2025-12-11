#include "core/ParticleSystem.h"
#include <algorithm>
#include <cmath>

// ----------------------------------------------------
// Constructor
// ----------------------------------------------------
ParticleSystem::ParticleSystem(size_t maxParticles)
    : maxCount(maxParticles)
{
    particles.reserve(maxParticles);
}

// ----------------------------------------------------
// Update
// ----------------------------------------------------
void ParticleSystem::update(float dt)
{
    // 1) Emitir partículas nuevas desde todos los emisores
    for (auto* e : emitters)
    {
        if (e)
            e->emit(*this, dt);
    }

    // 2) Actualizar cada partícula (fuerzas, vida, movimiento, suelo)
    for (auto& p : particles)
    {
        if (p.life <= 0.0f)
            continue;

        // Aplicar fuerzas externas
        for (auto* f : forces)
        {
            if (f)
                f->apply(p, dt);
        }

        // Envejecer
        p.life -= dt;
        if (p.life <= 0.0f)
            continue;

        // Integrar posición
        p.position = p.position + p.velocity * dt;

        // Colisión opcional con el "suelo"
        if (m_groundEnabled && p.position.y >= m_groundY)
        {
            p.position.y = m_groundY;

            if (p.velocity.y > 0.0f)
            {
                p.velocity.y = -p.velocity.y * m_groundRestitution;
            }
        }
    }

    // 2.5) Interacciones entre partículas (si están activadas)
    if (m_enableInteractions)
    {
        applyParticleInteractions(dt);
    }

    // 3) Eliminar partículas muertas
    particles.erase(
        std::remove_if(
            particles.begin(),
            particles.end(),
            [](const Particle& p) { return p.life <= 0.0f; }),
        particles.end());
}

// ----------------------------------------------------
// Emitir una nueva partícula
// ----------------------------------------------------
void ParticleSystem::emit(const Particle& p)
{
    if (particles.size() < maxCount)
    {
        particles.push_back(p);
    }
}

// ----------------------------------------------------
// Interacción entre partículas (repulsión suave)
// ----------------------------------------------------
void ParticleSystem::applyParticleInteractions(float dt)
{
    const float r  = m_interRadius;
    const float r2 = r * r;

    const size_t n = particles.size();

    for (size_t i = 0; i < n; ++i)
    {
        Particle& a = particles[i];
        if (a.life <= 0.0f)
            continue;

        for (size_t j = i + 1; j < n; ++j)
        {
            Particle& b = particles[j];
            if (b.life <= 0.0f)
                continue;

            // Vector entre partículas
            Vec2 d = b.position - a.position;
            float dist2 = d.x * d.x + d.y * d.y;

            // Demasiado cerca (evitar división por cero) o demasiado lejos
            if (dist2 < 0.0001f || dist2 > r2)
                continue;

            float dist = std::sqrt(dist2);
            float invd = 1.0f / dist;

            // Dirección normalizada
            Vec2 nrm = d * invd;

            // Fuerza proporcional a qué tan cerca están (más cerca = más fuerza)
            float factor = (1.0f - dist / r);   // en [0,1]
            float F = m_interStrength * factor;

            // Impulso (simétrico)
            Vec2 impulse = nrm * (F * dt);

            // Repulsión: se empujan en direcciones opuestas
            a.velocity = a.velocity - impulse;
            b.velocity = b.velocity + impulse;
        }
    }
}
