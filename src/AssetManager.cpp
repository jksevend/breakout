#include "AssetManager.h"

#include <format>
#include <utility>

#include "Utility.h"
#include "spdlog/spdlog.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

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

auto Toyengine::AssetManager::loadTextureFromFile(const std::string& path, bool alpha) -> Texture2D
{
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 3);
    if (data == nullptr)
    {
        const auto message = std::format("Failed to load texture '{}' with error: {}", path, stbi_failure_reason());
        throw std::runtime_error(message);
    }

    auto texture2d = Texture2D(width, height, data);

    if (alpha)
    {
        texture2d.setAlphaFormat();
    }

    stbi_image_free(data);

    return texture2d;
}

Toyengine::AssetManager::AssetManager()
{
}

Toyengine::AssetManager::~AssetManager()
{
    for (const auto& [name, shader] : this->m_Shaders)
    {
        spdlog::debug("Deleting shader '{}'", name);
        glDeleteProgram(shader.getId());
    }

    for (const auto& [name, texture] : this->m_Textures)
    {
        spdlog::debug("Deleting texture '{}'", name);
        auto id = texture.getId();
        glDeleteTextures(1, &id);
    }
}

auto Toyengine::AssetManager::loadShader(const std::string& name, std::string vertexPath, std::string fragmentPath,
                                         std::string geometryPath) -> Shader
{
    if (this->m_Shaders.contains(name))
    {
        const auto message = std::format("Shader with name '{}' already exists", name);
        throw std::runtime_error(message);
    }

    auto shader = this->loadShaderFromFile(std::move(vertexPath), std::move(fragmentPath), std::move(geometryPath));
    this->m_Shaders.insert(std::make_pair(name, shader));
    spdlog::debug("Shader '{}' loaded", name);
    return shader;
}

auto Toyengine::AssetManager::getShader(const std::string& name) -> Shader
{
    return this->m_Shaders.at(name);
}

auto Toyengine::AssetManager::loadTexture2D(const std::string& name, const std::string& path, bool alpha) -> Texture2D
{
    if (this->m_Textures.contains(name))
    {
        const auto message = std::format("Texture with name '{}' already exists", name);
        throw std::runtime_error(message);
    }

    auto texture2d = this->loadTextureFromFile(path, alpha);
    this->m_Textures.insert(std::make_pair(name, texture2d));
    spdlog::debug("Texture '{}' loaded", name);
    return texture2d;
}

auto Toyengine::AssetManager::getTexture2D(const std::string& name) -> Texture2D
{
    return this->m_Textures.at(name);
}
