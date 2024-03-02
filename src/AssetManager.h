#pragma once
#include <string>
#include <unordered_map>

#include "Shader.h"
#include "Texture2D.h"

namespace Toyengine
{
    class AssetManager
    {
        std::unordered_map<std::string, Shader> m_Shaders;
        std::unordered_map<std::string, Texture2D> m_Textures;

        auto loadShaderFromFile(std::string vertexPath, std::string fragmentPath,
                                std::string geometryPath = "") -> Shader;
        auto loadTextureFromFile(const std::string& path, bool alpha) -> Texture2D;

    public:
        AssetManager();
        ~AssetManager();

        auto loadShader(const std::string& name, std::string vertexPath, std::string fragmentPath,
                        std::string geometryPath = "") -> Shader;
        auto getShader(const std::string& name) -> Shader;

        auto loadTexture2D(const std::string& name, const std::string& path, bool alpha) -> Texture2D;
        auto getTexture2D(const std::string& name) -> Texture2D;
    };
}
