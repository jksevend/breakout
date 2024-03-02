#pragma once
#include "GameEntity.h"

namespace Toyengine
{
    class BallObject : public GameEntity
    {
        float m_Radius;
        bool m_Stuck;
    public:
        BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);

        auto move(float dt, unsigned int windowWidth) -> glm::vec2;
        auto reset(glm::vec2 position, glm::vec2 velocity) -> void;

        [[nodiscard]] auto isStuck() const -> bool;
        auto setStuck(bool stuck) -> void;
    };
}
