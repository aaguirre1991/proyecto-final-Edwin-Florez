#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <optional>

#include <SFML/Graphics.hpp>

#include "../include/core/ParticleSystem.h"
#include "../include/core/ExplosionEmitter.h"
#include "../include/core/GravityForce.h"
#include "../include/core/TurbulenceForce.h"

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Particle System - Sparks Explosion Demo"
    );
    window.setFramerateLimit(60);

    // Sistema de partículas
    ParticleSystem system(800);

    // Emisor en el centro
    ExplosionEmitter explosion(
    Vec2{400.f, 300.f},  // origen de la explosión
    240.f,               // velocidad base de las chispas
    900                  // nº de partículas
);
    system.addEmitter(&explosion);

    GravityForce gravity(Vec2{0.0f, 220.0f});
    system.addForce(&gravity);

    TurbulenceForce turbulence(80.0f);
    system.addForce(&turbulence);

    sf::CircleShape sparkShape(2.0f);
    sparkShape.setOrigin({2.0f, 2.0f});

    sf::Clock clock;
    float timeSinceLastExplosion = 0.0f;
    const float explosionInterval = 1.5f; // una explosión cada 1.5 s

    // ---- primera explosión nada más iniciar ----
    explosion.trigger();

    while (window.isOpen())
    {
        // ---- Eventos ----
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                if (key->scancode == sf::Keyboard::Scancode::Space)
                    explosion.trigger();      // explosión manual con espacio
            }

           if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>())
{
    if (mouse->button == sf::Mouse::Button::Left)
    {
        // Posicionar la explosión en el punto donde se hizo clic
        explosion.setOrigin(Vec2{
            static_cast<float>(mouse->position.x),
            static_cast<float>(mouse->position.y)
        });

        explosion.trigger();     // ¡BOOM!
    }
}

        }

        float dt = clock.restart().asSeconds();
        timeSinceLastExplosion += dt;

        // ---- explosión automática cada cierto tiempo ----
        if (timeSinceLastExplosion >= explosionInterval)
        {
            timeSinceLastExplosion = 0.0f;
            explosion.trigger();
        }

        system.update(dt);

        window.clear(sf::Color::Black);

        const auto& particles = system.getParticles();
        for (const auto& p : particles)
        {
            float t = 1.0f - (p.life / p.maxLife);
            if (t < 0.0f) t = 0.0f;
            if (t > 1.0f) t = 1.0f;

            std::uint8_t r = static_cast<std::uint8_t>(50.0f  * (1.0f - t));
            std::uint8_t g = static_cast<std::uint8_t>(200.0f * (1.0f - 0.3f * t));
            std::uint8_t b = static_cast<std::uint8_t>(255.0f);

            sparkShape.setFillColor(sf::Color(r, g, b, 255));
            sparkShape.setPosition({p.position.x, p.position.y});
            window.draw(sparkShape);
        }

        window.display();
    }

    return 0;
}
