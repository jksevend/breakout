#pragma once
#include <string>
#include <unordered_map>

#include "Shader.h"

namespace Toyengine
{
    class AssetManager
    {
        std::unordered_map<std::string, Shader> m_Shaders;

        auto loadShaderFromFile(std::string vertexPath, std::string fragmentPath,
                                std::string geometryPath = "") -> Shader;

    public:
        AssetManager();
        ~AssetManager();

        auto loadShader(const std::string& name, std::string vertexPath, std::string fragmentPath,
                        std::string geometryPath = "") -> Shader;
        auto getShader(const std::string& name) -> Shader;
    };
}
