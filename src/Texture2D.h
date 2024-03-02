#pragma once

#include <glad/glad.h>

namespace Toyengine
{
    class Texture2D
    {
        unsigned int m_Id;
        unsigned int m_Width, m_Height;

        unsigned int m_InternalFormat, m_ImageFormat;
        unsigned int m_WrapS, m_WrapT;
        unsigned int m_FilterMin, m_FilterMax;

    public:
        [[nodiscard]] auto getId() const -> unsigned int;
        Texture2D();
        Texture2D(unsigned int width, unsigned int height, const unsigned char* data);
        ~Texture2D() = default;

        auto bind() const -> void;
        auto setAlphaFormat() -> void;
    };
}
