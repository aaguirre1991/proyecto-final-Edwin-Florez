#pragma once

#include "Vec2.h"

struct Particle {
    Vec2 position;    // posición
    Vec2 velocity;    // velocidad
    float life{0.0f};     // tiempo restante
    float maxLife{0.0f};  // tiempo total de vida
};
