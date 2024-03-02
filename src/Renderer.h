#pragma once
#include "Shader.h"
#include "Texture2D.h"

namespace Toyengine
{
    class Renderer
    {
        unsigned int m_VertexArray;
        Shader m_Shader;

    public:
        explicit Renderer(const Shader& shader);
        ~Renderer();

        auto drawSprite(const Texture2D& texture,
                        glm::vec2 position,
                        glm::vec2 size = glm::vec2(10.0f, 10.0f),
                        glm::vec3 color = glm::vec3(1.0f),
                        float rotate = 0.0f) -> void;
    };
}
