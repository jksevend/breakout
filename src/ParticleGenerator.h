#pragma once
#include <vector>

#include "GameEntity.h"
#include "Particle.h"
#include "Shader.h"
#include "Texture2D.h"

namespace Toyengine
{
    class ParticleGenerator
    {
        std::vector<Particle> m_Particles;
        unsigned int m_Amount;

        Shader m_Shader;
        Texture2D m_Texture;
        unsigned int m_VertexArray;

        auto firstUnusedParticle() -> int;
        auto respawnParticle(Particle& particle, GameEntity* entity, glm::vec2 offset = glm::vec2(0.0f, 0.0f)) -> void;

    public:
        ParticleGenerator(const Shader& shader, const Texture2D& texture, unsigned int amount);

        auto update(float deltaTime, GameEntity* entity, unsigned int newParticles,
                    glm::vec2 offset = glm::vec2(0.0f, 0.0f)) -> void;
        auto draw() -> void;
    };
}
