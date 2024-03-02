#include "GameEntity.h"

Toyengine::GameEntity::GameEntity(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color,
                                  glm::vec2 velocity) : m_Position(pos), m_Size(size), m_Velocity(velocity),
                                                        m_Color(color), m_Rotation(0.0f), m_IsSolid(false),
                                                        m_IsDestroyed(false), m_Sprite(sprite)

{
}

auto Toyengine::GameEntity::setSolid(const bool solid) -> void
{
    this->m_IsSolid = solid;
}

auto Toyengine::GameEntity::isSolid() const -> bool
{
    return this->m_IsSolid;
}

auto Toyengine::GameEntity::isDestroyed() const -> bool
{
    return this->m_IsDestroyed;
}

auto Toyengine::GameEntity::setDestroyed(bool value) -> void
{
    this->m_IsDestroyed = value;
}

auto Toyengine::GameEntity::getPosition() const -> glm::vec2
{
    return this->m_Position;
}

auto Toyengine::GameEntity::setPosition(glm::vec2 value) -> void
{
    this->m_Position = value;
}

auto Toyengine::GameEntity::translate(const glm::vec2 vector2) -> void
{
    this->m_Position.x = this->m_Position.x + vector2.x;
    this->m_Position.y = this->m_Position.y + vector2.y;
}

auto Toyengine::GameEntity::getSize() -> glm::vec2
{
    return this->m_Size;
}

auto Toyengine::GameEntity::setSize(glm::vec2 value) -> void
{
    this->m_Size = value;
}

auto Toyengine::GameEntity::getVelocity() -> glm::vec2
{
    return this->m_Velocity;
}

auto Toyengine::GameEntity::setVelocity(glm::vec2 value) -> void
{
    this->m_Velocity = value;
}

void Toyengine::GameEntity::draw(const std::unique_ptr<Renderer>& renderer)
{
    renderer->drawSprite(this->m_Sprite, this->m_Position, this->m_Size, this->m_Color, this->m_Rotation);
}
