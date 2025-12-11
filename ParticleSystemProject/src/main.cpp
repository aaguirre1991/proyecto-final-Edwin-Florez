#include <cstdint>
#include <cstdlib>
#include <ctime>

#include <SFML/Graphics.hpp>
#include <optional>    // std::optional

#include "../include/core/ParticleSystem.h"
#include "../include/core/FireEmitter.h"
#include "../include/core/GravityForce.h"
#include "../include/core/TurbulenceForce.h"

int main()
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Crear ventana (en SFML 3.0 VideoMode recibe un Vector2u)
    sf::RenderWindow window(
        sf::VideoMode({800u, 600u}),
        "Particle System - Fire Demo"
    );
    window.setFramerateLimit(60);

    // Sistema de partículas
    ParticleSystem system(300);

    // Emisor de fuego: colocamos las partículas cerca de la parte baja de la ventana
    FireEmitter fire(Vec2{400.0f, 550.0f}, 60.0f); // posición en píxeles (x, y)
    system.addEmitter(&fire);

    // Intensidad inicial del fuego
    float intensity = 1.0f;
    fire.setIntensity(intensity);

    // “Flotabilidad”: empuja hacia arriba (en coordenadas de pantalla, arriba es y negativa)
    GravityForce buoyancy(Vec2{0.0f, -50.0f});
    system.addForce(&buoyancy);

    // Turbulencia (viento aleatorio)
    TurbulenceForce turbulence(40.0f);
    system.addForce(&turbulence);

    // Forma gráfica de cada partícula (reutilizada para todas)
    sf::CircleShape particleShape(4.0f);   // radio
    particleShape.setOrigin({4.f, 4.f});   // centro en el medio

    sf::Clock clock;

    while (window.isOpen())
    {
        // ---- Manejo de eventos (SFML 3) ----
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                using Scan = sf::Keyboard::Scancode;

                if (keyPressed->scancode == Scan::Up || keyPressed->scancode == Scan::W)
                {
                    intensity = std::min(intensity + 0.25f, 5.0f);
                    fire.setIntensity(intensity);
                    window.setTitle("Particle System - Fire Demo (intensity = "
                                    + std::to_string(intensity) + ")");
                }
                else if (keyPressed->scancode == Scan::Down || keyPressed->scancode == Scan::S)
                {
                    intensity = std::max(intensity - 0.25f, 0.0f);
                    fire.setIntensity(intensity);
                    window.setTitle("Particle System - Fire Demo (intensity = "
                                    + std::to_string(intensity) + ")");
                }
            }
        }

        // ---- Tiempo delta ----
        float dt = clock.restart().asSeconds();

        // Actualizar sistema de partículas
        system.update(dt);

        // ---- Dibujado ----
        window.clear(sf::Color::Black);

        const auto& particles = system.getParticles();

        for (const auto& p : particles)
        {
            // 0 = recién emitida, 1 = a punto de morir
            float age = 1.0f - (p.life / p.maxLife);
            if (age < 0.f) age = 0.f;
            if (age > 1.f) age = 1.f;

            std::uint8_t r, g, b;

            if (age < 0.3f) // muy joven: blanco-amarillo
            {
                float t = age / 0.3f; // 0..1
                r = 255;
                g = 255;
                b = static_cast<std::uint8_t>(200.f + 55.f * t); // 200 -> 255
            }
            else if (age < 0.7f) // media: naranja
            {
                float t = (age - 0.3f) / 0.4f; // 0..1
                r = 255;
                g = static_cast<std::uint8_t>(255.f - 155.f * t); // 255 -> 100
                b = 50;
            }
            else // vieja: rojo oscuro / humo
            {
                float t = (age - 0.7f) / 0.3f; // 0..1
                r = static_cast<std::uint8_t>(255.f - 155.f * t); // 255 -> 100
                g = static_cast<std::uint8_t>(100.f - 80.f * t);  // 100 -> 20
                b = static_cast<std::uint8_t>(50.f  + 30.f * t);  // 50  -> 80
            }

            // Se desvanece al morir
            std::uint8_t alpha = static_cast<std::uint8_t>(255.f * (1.0f - age));

            particleShape.setFillColor(sf::Color(r, g, b, alpha));
            particleShape.setPosition({p.position.x, p.position.y});

            window.draw(particleShape);
        }

        window.display();
    }

    return 0;
}
