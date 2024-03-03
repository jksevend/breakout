#include "Renderer.h"

Toyengine::Renderer::Renderer(const Shader& shader) : m_Shader(shader)
{
    unsigned int vertexBuffer;
    constexpr float vertices[] = {
        // pos      // tex
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &this->m_VertexArray);
    glGenBuffers(1, &vertexBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->m_VertexArray);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(nullptr));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Toyengine::Renderer::~Renderer()
{
    glDeleteVertexArrays(1, &this->m_VertexArray);
}

auto Toyengine::Renderer::drawSprite(const Texture2D& texture, const glm::vec2 position, const glm::vec2 size,
                                     const glm::vec3 color,
                                     const float rotate) -> void
{
    this->m_Shader.activate();

    auto model = glm::mat4(1.0f);
    model = translate(model, glm::vec3(position, 0.0f));
    model = translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    model = translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

    model = scale(model, glm::vec3(size, 1.0f)); // last scale

    this->m_Shader.setMatrix4("model", model);
    this->m_Shader.setVector3f("spriteColor", color);

    texture.bind();

    glBindVertexArray(this->m_VertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}
