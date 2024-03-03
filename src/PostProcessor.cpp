//
// Created by Julian on 03.03.2024.
//

#include "PostProcessor.h"

Toyengine::PostProcessor::PostProcessor(Shader shader, unsigned int width, unsigned int height) : m_Shader(shader),
    m_Texture(), m_Width(width), m_Height(height), m_Confuse(false), m_Chaos(false), m_Shake(false)
{
    glGenFramebuffers(1, &this->m_MultiSampledFrameBuffer);
    glGenFramebuffers(1, &this->m_FrameBuffer);
    glGenRenderbuffers(1, &this->m_RenderBuffer);
    // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_MultiSampledFrameBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, this->m_RenderBuffer);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height);
    // allocate storage for render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->m_RenderBuffer);
    // attach MS render buffer object to framebuffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
    }
    // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_FrameBuffer);
    this->m_Texture = Texture2D(width, height, nullptr, false);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->m_Texture.getId(), 0);
    // attach texture to framebuffer as its color attachment
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->m_vertexArray);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->m_vertexArray);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void*>(0));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    this->m_Shader.setInteger("scene", 0, true);
    float offset = 1.0f / 300.0f;
    float offsets[9][2] = {
        {-offset, offset}, // top-left
        {0.0f, offset}, // top-center
        {offset, offset}, // top-right
        {-offset, 0.0f}, // center-left
        {0.0f, 0.0f}, // center-center
        {offset, 0.0f}, // center - right
        {-offset, -offset}, // bottom-left
        {0.0f, -offset}, // bottom-center
        {offset, -offset} // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->m_Shader.getId(), "offsets"), 9, reinterpret_cast<float*>(offsets));
    int edgeKernel[9] = {
        -1, -1, -1,
        -1, 8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(this->m_Shader.getId(), "edge_kernel"), 9, edgeKernel);
    float blurKernel[9] = {
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
        2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
        1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    };
    glUniform1fv(glGetUniformLocation(this->m_Shader.getId(), "blur_kernel"), 9, blurKernel);
}

auto Toyengine::PostProcessor::beginRender() -> void
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->m_MultiSampledFrameBuffer);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

auto Toyengine::PostProcessor::endRender() -> void
{
    // now resolve multisampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->m_MultiSampledFrameBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->m_FrameBuffer);
    glBlitFramebuffer(0, 0, this->m_Width, this->m_Height, 0, 0, this->m_Width, this->m_Height, GL_COLOR_BUFFER_BIT,
                      GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

auto Toyengine::PostProcessor::render(float time) -> void
{
    // set uniforms/options
    this->m_Shader.activate();
    this->m_Shader.setFloat("time", time);
    this->m_Shader.setInteger("confuse", this->m_Confuse);
    this->m_Shader.setInteger("chaos", this->m_Chaos);
    this->m_Shader.setInteger("shake", this->m_Shake);
    // render textured quad
    glActiveTexture(GL_TEXTURE0);
    this->m_Texture.bind();
    glBindVertexArray(this->m_vertexArray);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

auto Toyengine::PostProcessor::setShake(bool value) -> void
{
    this->m_Shake = value;
}
