#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <optional>

#include "../include/core/ParticleSystem.h"
#include "../include/core/RainEmitter.h"
#include "../include/core/GravityForce.h"
#include "../include/core/TurbulenceForce.h"

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Particle System - Rain Demo"
    );
    window.setFramerateLimit(60);

    const float windowWidth  = 800.0f;
    const float windowHeight = 600.0f;

    ParticleSystem system(4000);

    // activar interacciones
    system.setInteractionsEnabled(true);
    system.setInteractionRadius(30.f);     // probar valores 20–40
    system.setInteractionStrength(150.f);  // probar 80–200

    // ----------------------------------------------------------
    // EMISOR TIPO FUENTE:
    // área muy pequeña en la parte inferior, centrada
    // ----------------------------------------------------------
    Vec2 fountainMin{ windowWidth * 0.5f - 8.0f, windowHeight - 55.0f };
    Vec2 fountainMax{ windowWidth * 0.5f + 8.0f, windowHeight - 50.0f };

    // Velocidad base hacia ARRIBA (chorro principal)
    Vec2 baseVel{ 0.0f, -550.0f };

    // Menos partículas para que no sea un “bloque” sólido
    float rate = 700.0f;

    RainEmitter rain(
        fountainMin,
        fountainMax,
        rate,
        baseVel
    );
    system.addEmitter(&rain);

    // Gravedad fuerte hacia abajo para arcos más marcados
    GravityForce gravity(Vec2{0.0f, 900.0f});
    system.addForce(&gravity);

    // Turbulencia un poco mayor para romper la columna
    TurbulenceForce turbulence(70.0f);
    system.addForce(&turbulence);

    // Suelo alrededor de y = 580
    system.enableGround(580.0f, 0.2f);

    // En vez de gota alargada, usamos un círculo pequeño (salpicaduras)
    sf::CircleShape dropShape(3.0f);
    dropShape.setOrigin({3.0f, 3.0f});

    sf::Clock clock;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        float dt = clock.restart().asSeconds();

        system.update(dt);

        window.clear(sf::Color::Black);

        const auto& particles = system.getParticles();

        for (const auto& p : particles)
        {
            if (p.life <= 0.0f) continue;

            float t = 1.0f - (p.life / p.maxLife);
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            // Azul/blanco según la vida
            std::uint8_t r = 80;
            std::uint8_t g = static_cast<std::uint8_t>(150 + 80 * t);
            std::uint8_t b = static_cast<std::uint8_t>(210 + 45 * t);

            dropShape.setFillColor(sf::Color(r, g, b, 255));
            dropShape.setPosition({p.position.x, p.position.y});

            window.draw(dropShape);
        }

        window.display();
    }

    return 0;
}
