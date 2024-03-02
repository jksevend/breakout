#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Renderer.h"
#include "Texture2D.h"

namespace Toyengine
{
    class GameEntity
    {
        glm::vec3 m_Color;
        float m_Rotation;
        bool m_IsSolid;
        bool m_IsDestroyed;
        Texture2D m_Sprite;

    protected:
        glm::vec2 m_Position, m_Size, m_Velocity;
    public:
        GameEntity(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f),
                   glm::vec2 velocity = glm::vec2(0.0f, 0.0f));

        auto setSolid(bool solid) -> void;
        [[nodiscard]] auto isSolid() const -> bool;

        [[nodiscard]] auto isDestroyed() const -> bool;
        auto setDestroyed(bool value) -> void;

        [[nodiscard]] auto getPosition() const -> glm::vec2;
        auto translate(glm::vec2 vector2) -> void;
        auto getSize() -> glm::vec2;

        virtual void draw(const std::unique_ptr<Renderer>& renderer);
    };
}
