//
// Created by Julian on 02.03.2024.
//

#include "BallObject.h"

Toyengine::BallObject::BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite) :
    GameEntity(pos, glm::vec2(radius * 2.0f, radius * 2.0f), sprite, glm::vec3(1.0f), velocity), m_Radius(radius),
    m_Stuck(true)
{
}

auto Toyengine::BallObject::move(float dt, unsigned int windowWidth) -> glm::vec2
{
    // if not stuck to player board
    if (!this->m_Stuck)
    {
        // move the ball
        this->m_Position += this->m_Velocity * dt;
        // then check if outside window bounds and if so, reverse velocity and restore at correct position
        if (this->m_Position.x <= 0.0f)
        {
            this->m_Position.x = -this->m_Velocity.x;
            this->m_Position.x = 0.0f;
        }
        else if (this->m_Position.x + this->m_Size.x >= static_cast<float>(windowWidth))
        {
            this->m_Velocity.x = -this->m_Velocity.x;
            this->m_Position.x = static_cast<float>(windowWidth) - this->m_Size.x;
        }
        if (this->m_Position.y <= 0.0f)
        {
            this->m_Velocity.y = -this->m_Velocity.y;
            this->m_Position.y = 0.0f;
        }
    }
    return this->m_Position;
}

auto Toyengine::BallObject::reset(glm::vec2 position, glm::vec2 velocity) -> void
{
    this->m_Position = position;
    this->m_Velocity = velocity;
    this->m_Stuck = true;
}

auto Toyengine::BallObject::isStuck() const -> bool
{
    return this->m_Stuck;
}

auto Toyengine::BallObject::setStuck(bool stuck) -> void
{
    this->m_Stuck = stuck;
}
