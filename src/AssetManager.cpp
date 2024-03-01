#include "AssetManager.h"

#include <format>
#include <utility>

#include "Utility.h"
#include "spdlog/spdlog.h"

auto Toyengine::AssetManager::loadShaderFromFile(std::string vertexPath, std::string fragmentPath,
                                                 std::string geometryPath) -> Shader
{
    const auto vertexShaderCode = Utility::loadFile(std::move(vertexPath));
    const auto fragmentShaderCode = Utility::loadFile(std::move(fragmentPath));

    std::string geometryShaderCode;
    if (!geometryPath.empty())
    {
        geometryShaderCode = Utility::loadFile(std::move(geometryPath));
    }

    auto shader = Shader(vertexShaderCode, fragmentShaderCode, geometryShaderCode);
    return shader;
}

Toyengine::AssetManager::AssetManager()
{
}

Toyengine::AssetManager::~AssetManager()
{
    for (const auto& [name, shader] : this->m_Shaders)
    {
        spdlog::debug("Deleting shaders {}", name);
        glDeleteProgram(shader.getId());
    }
}

auto Toyengine::AssetManager::loadShader(const std::string& name, std::string vertexPath, std::string fragmentPath,
                                         std::string geometryPath) -> Shader
{
    if (this->m_Shaders.contains(name))
    {
        const auto message = std::format("Shader with name {} already exists", name);
        throw std::runtime_error(message);
    }

    auto shader = this->loadShaderFromFile(std::move(vertexPath), std::move(fragmentPath), std::move(geometryPath));
    this->m_Shaders.insert(std::make_pair(name, shader));
    spdlog::debug("Shader {} loaded", name);
}

auto Toyengine::AssetManager::getShader(const std::string& name) -> Shader
{
    return this->m_Shaders.at(name);
}
