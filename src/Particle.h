#pragma once
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>


namespace Toyengine
{
    class Particle
    {
        glm::vec2 m_Position, m_Velocity;
        glm::vec4 m_Color;
        float m_Life;

    public:
        Particle();

        [[nodiscard]] auto getPosition() const -> glm::vec2;
        auto setPosition(glm::vec2 position) -> void;
        [[nodiscard]] auto getVelocity() const -> glm::vec2;
        auto setVelocity(glm::vec2 velocity) -> void;
        [[nodiscard]] auto getColor() const -> glm::vec4;
        auto setColor(glm::vec4 color) -> void;
        [[nodiscard]] auto getLife() const -> float;
        auto setLife(float life) -> void;

    };
}
