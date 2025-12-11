#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <optional>
#include <cmath>

#include <SFML/Graphics.hpp>

#include "../include/core/ParticleSystem.h"
#include "../include/core/FireEmitter.h"
#include "../include/core/RainEmitter.h"
#include "../include/core/ExplosionEmitter.h"
#include "../include/core/GravityForce.h"
#include "../include/core/TurbulenceForce.h"

// Pequeña constante para PI
constexpr float PI = 3.14159265359f;

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Particle System - Firework Scene"
    );
    window.setFramerateLimit(60);

    const float windowWidth  = 800.0f;
    const float windowHeight = 600.0f;

    // =========================================================
    // 1) SISTEMA DEL COHETE (FUEGO QUE SUBE)
    // =========================================================
    ParticleSystem fireSystem(250);

    // Emisor de fuego en la parte baja
    FireEmitter fire(Vec2{windowWidth * 0.5f, windowHeight - 50.f}, 70.f);
    fireSystem.addEmitter(&fire);

    // Flotación hacia arriba
    GravityForce fireBuoyancy(Vec2{0.f, -80.f});
    fireSystem.addForce(&fireBuoyancy);

    // Turbulencia suave
    TurbulenceForce fireTurb(18.f);
    fireSystem.addForce(&fireTurb);

    // Interacciones suaves (para que no rompan la silueta)
    fireSystem.setInteractionsEnabled(true);
    fireSystem.setInteractionRadius(20.0f);
    fireSystem.setInteractionStrength(70.0f);

    // =========================================================
    // 2) SISTEMA DE EXPLOSIONES (CHISPAS)
    // =========================================================
    ParticleSystem explosionSystem(600);

    // La explosión ocurrirá en la parte alta
    ExplosionEmitter explosion(Vec2{windowWidth * 0.5f, 220.f}, 260.f, 800);
    explosionSystem.addEmitter(&explosion);

    GravityForce explosionGravity(Vec2{0.f, 200.f});
    explosionSystem.addForce(&explosionGravity);

    TurbulenceForce explosionTurb(60.f);
    explosionSystem.addForce(&explosionTurb);

    // Control de múltiples explosiones
    int   explosionsTriggered   = 0;
    float nextExplosionTime     = 2.0f;  // primera explosión a los 2 s
    const float explosionInterval = 0.8f; // separación entre explosiones

    // =========================================================
    // 3) SISTEMA DE "LLUVIA" TIPO FUENTE
    // =========================================================
    ParticleSystem rainSystem(1200);

    // Boca de la fuente cerca de la parte inferior, centrada
    Vec2 fountainMin{ windowWidth * 0.5f - 8.0f, windowHeight - 55.0f };
    Vec2 fountainMax{ windowWidth * 0.5f + 8.0f, windowHeight - 50.0f };

    // Velocidad base hacia ARRIBA (chorro principal)
    Vec2 baseVel{ 0.0f, -550.0f };

    RainEmitter rain(
        fountainMin,   // areaMin: boca de la fuente
        fountainMax,   // areaMax: boca de la fuente
        700.f,         // partículas por segundo
        baseVel        // velocidad base hacia arriba
    );
    rainSystem.addEmitter(&rain);

    // Gravedad fuerte hacia abajo para las parábolas
    GravityForce rainGravity(Vec2{0.f, 900.f});
    rainSystem.addForce(&rainGravity);

    // Turbulencia para romper un poco la columna
    TurbulenceForce rainTurb(70.f);
    rainSystem.addForce(&rainTurb);

    // Rebote en el suelo para salpicaduras
    rainSystem.enableGround(580.0f, 0.2f);

    // Interacciones de la fuente
   rainSystem.setInteractionsEnabled(true);
   rainSystem.setInteractionRadius(30.0f);
   rainSystem.setInteractionStrength(150.0f);

    bool rainStarted = false;

    // =========================================================
    // 4) RELOJ Y LOOP PRINCIPAL
    // =========================================================
    sf::Clock clock;
    float time = 0.f;

    while (window.isOpen())
    {
        // Eventos SFML 3
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();
        time += dt;

        // -----------------------------------------------------
        // Lógica de escenas
        // -----------------------------------------------------

        // Disparar hasta 3 explosiones separadas por explosionInterval
        if (explosionsTriggered < 3 && time >= nextExplosionTime)
        {
            explosion.setOrigin(Vec2{windowWidth * 0.5f, 220.f});
            explosion.trigger();
            ++explosionsTriggered;
            nextExplosionTime += explosionInterval;
        }

        // Empezar la fuente DESPUÉS de la tercera explosión
        // (dejamos un pequeño margen de 0.5 s)
        if (!rainStarted && explosionsTriggered >= 3 && time >= nextExplosionTime + 0.5f)
        {
            rainStarted = true;
        }

        // -----------------------------------------------------
        // Actualizar sistemas de partículas
        // -----------------------------------------------------
        if (time < 2.5f)               // mientras el cohete "sube"
            fireSystem.update(dt);

        if (explosionsTriggered > 0)   // al menos una explosión disparada
            explosionSystem.update(dt);

        if (rainStarted)               // fuente al final
            rainSystem.update(dt);

        // =====================================================
        // 5) DIBUJO
        // =====================================================
        window.clear();

        // ----- Fuego (cohete) -----
        if (time < 2.5f)
        {
            const auto& particles = fireSystem.getParticles();
            sf::CircleShape shape(4.f);
            shape.setOrigin({4.f, 4.f});

            for (const auto& p : particles)
            {
                if (p.life <= 0.f) continue;

                float t = 1.0f - (p.life / p.maxLife);
                if (t < 0.f) t = 0.f;
                if (t > 1.f) t = 1.f;

                std::uint8_t r = static_cast<std::uint8_t>(230.f * (1.f - t));
                std::uint8_t g = static_cast<std::uint8_t>(180.f * (1.f - t));
                std::uint8_t b = static_cast<std::uint8_t>( 40.f * (1.f - t));

                shape.setFillColor(sf::Color(r, g, b, 255));
                shape.setPosition({p.position.x, p.position.y});
                window.draw(shape);
            }
        }

        // ----- Explosiones de chispas -----
        if (explosionsTriggered > 0)
        {
            const auto& particles = explosionSystem.getParticles();
            sf::CircleShape spark(2.5f);
            spark.setOrigin({2.5f, 2.5f});

            for (const auto& p : particles)
            {
                if (p.life <= 0.f) continue;

                float t = 1.0f - (p.life / p.maxLife);
                if (t < 0.f) t = 0.f;
                if (t > 1.f) t = 1.f;

                std::uint8_t r = static_cast<std::uint8_t>( 40.f + 215.f * (1.f - t));
                std::uint8_t g = static_cast<std::uint8_t>(100.f + 155.f * (1.f - t));
                std::uint8_t b = static_cast<std::uint8_t>(255.f * t);

                spark.setFillColor(sf::Color(r, g, b, 255));
                spark.setPosition({p.position.x, p.position.y});
                window.draw(spark);
            }
        }

        // ----- Fuente de agua (antes "lluvia") -----
        if (rainStarted)
        {
            const auto& particles = rainSystem.getParticles();

            sf::CircleShape drop(3.f);
            drop.setOrigin({3.f, 3.f});

            for (const auto& p : particles)
            {
                if (p.life <= 0.f) continue;

                float t = 1.0f - (p.life / p.maxLife);
                if (t < 0.f) t = 0.f;
                if (t > 1.f) t = 1.f;

                std::uint8_t r = 80;
                std::uint8_t g = static_cast<std::uint8_t>(150 + 80 * t);
                std::uint8_t b = static_cast<std::uint8_t>(210 + 45 * t);

                drop.setFillColor(sf::Color(r, g, b, 255));
                drop.setPosition({p.position.x, p.position.y});

                window.draw(drop);
            }
        }

        window.display();
    }

    return 0;
}
