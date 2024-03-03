#pragma once
#include "Shader.h"
#include "Texture2D.h"

namespace Toyengine
{
    class PostProcessor
    {
        Shader m_Shader;
        Texture2D m_Texture;

        unsigned int m_Width, m_Height;
        bool m_Confuse, m_Chaos, m_Shake;

        unsigned int m_MultiSampledFrameBuffer, m_FrameBuffer;
        unsigned int m_RenderBuffer;
        unsigned int m_vertexArray;

    public:
        PostProcessor(Shader shader, unsigned int width, unsigned int height);

        auto beginRender() -> void;
        auto endRender() -> void;
        auto render(float time) -> void;

        auto setShake(bool value) -> void;
    };
}
