#include "Texture2D.h"

#include "spdlog/spdlog.h"

auto Toyengine::Texture2D::getId() const -> unsigned int
{
    return this->m_Id;
}

Toyengine::Texture2D::Texture2D()
{
}

Toyengine::Texture2D::Texture2D(const unsigned int width, const unsigned int height, const unsigned char* data) :
    m_Width(width),
    m_Height(height), m_InternalFormat(GL_RGB), m_ImageFormat(GL_RGB), m_WrapS(GL_REPEAT), m_WrapT(GL_REPEAT),
    m_FilterMin(GL_LINEAR_MIPMAP_LINEAR), m_FilterMax(GL_LINEAR)
{
    glGenTextures(1, &this->m_Id);
    this->bind();

    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 static_cast<int>(this->m_InternalFormat),
                 static_cast<int>(this->m_Width),
                 static_cast<int>(this->m_Height),
                 0,
                 this->m_ImageFormat,
                 GL_UNSIGNED_BYTE,
                 data);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<int>(this->m_WrapS));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<int>(this->m_WrapT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<int>(this->m_FilterMin));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<int>(this->m_FilterMax));

    // unbind texture
}

auto Toyengine::Texture2D::bind() const -> void
{
    glBindTexture(GL_TEXTURE_2D, this->m_Id);
}

auto Toyengine::Texture2D::setAlphaFormat() -> void
{
    this->m_InternalFormat = GL_RGBA;
    this->m_ImageFormat = GL_RGBA;
}
