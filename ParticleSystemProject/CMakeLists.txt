cmake_minimum_required(VERSION 3.16)
project(ParticleSystemProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------- 1. RUTA A SFML (ajustada a tu carpeta) ----------
set(SFML_ROOT "${CMAKE_SOURCE_DIR}/SFML-3.0.0-windows-vc17-64-bit/SFML-3.0.0")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIB_DIR     "${SFML_ROOT}/lib")

# ---------- 2. EJECUTABLES ----------

# Fuego (tu demo original)
add_executable(particles
    src/main.cpp
    src/ParticleSystem.cpp
    src/BasicEmitter.cpp
    src/FireEmitter.cpp
    src/TurbulenceForce.cpp
    src/GravityForce.cpp
)

# Lluvia
add_executable(particles_rain
    src/main_rain.cpp
    src/ParticleSystem.cpp
    src/BasicEmitter.cpp
    src/RainEmitter.cpp
    src/TurbulenceForce.cpp
    src/GravityForce.cpp
)

# Explosión eléctrica
add_executable(particles_explosion
    src/main_explosion.cpp
    src/ParticleSystem.cpp
    src/BasicEmitter.cpp
    src/ExplosionEmitter.cpp
    src/TurbulenceForce.cpp
    src/GravityForce.cpp
)

# Escena combinada fuego + explosión + lluvia
add_executable(particles_scene
    src/main_scene.cpp
    src/ParticleSystem.cpp
    src/BasicEmitter.cpp
    src/FireEmitter.cpp
    src/RainEmitter.cpp
    src/ExplosionEmitter.cpp
    src/TurbulenceForce.cpp
    src/GravityForce.cpp
)


# ---------- 3. INCLUDES Y LIBRERÍAS SFML ----------
foreach(target
        particles
        particles_rain
        particles_explosion
        particles_scene)

    target_include_directories(${target} PRIVATE
        ${SFML_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/include
    )

    target_link_directories(${target} PRIVATE
        ${SFML_LIB_DIR}
    )

    target_link_libraries(${target}
        sfml-graphics
        sfml-window
        sfml-system
    )
endforeach()
