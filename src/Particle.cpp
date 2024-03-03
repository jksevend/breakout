#include "Particle.h"

Toyengine::Particle::Particle() : m_Position(0.0f), m_Velocity(0.0f), m_Color(1.0f), m_Life(0.0f)
{
}

auto Toyengine::Particle::getPosition() const -> glm::vec2
{
    return this->m_Position;
}

auto Toyengine::Particle::setPosition(glm::vec2 position) -> void
{
    this->m_Position = position;
}

auto Toyengine::Particle::getVelocity() const -> glm::vec2
{
    return this->m_Velocity;
}

auto Toyengine::Particle::setVelocity(glm::vec2 velocity) -> void
{
    this->m_Velocity = velocity;
}

auto Toyengine::Particle::getColor() const -> glm::vec4
{
    return this->m_Color;
}

auto Toyengine::Particle::setColor(glm::vec4 color) -> void
{
    this->m_Color = color;
}

auto Toyengine::Particle::getLife() const -> float
{
    return this->m_Life;
}

auto Toyengine::Particle::setLife(float life) -> void
{
    this->m_Life = life;
}
