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

void Toyengine::GameEntity::draw(const std::unique_ptr<Renderer>& renderer)
{
    renderer->drawSprite(this->m_Sprite, this->m_Position, this->m_Size, this->m_Color, this->m_Rotation);
}
