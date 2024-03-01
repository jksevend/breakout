#include "Shader.h"

#include "spdlog/spdlog.h"

auto Toyengine::Shader::getGlShaderType(const ShaderType type) -> unsigned int
{
    switch (type)
    {
    case ShaderType::VERTEX:
        return GL_VERTEX_SHADER;
    case ShaderType::FRAGMENT:
        return GL_FRAGMENT_SHADER;
    case ShaderType::GEOMETRY:
        return GL_GEOMETRY_SHADER;
    }
}

auto Toyengine::Shader::compile(const std::string& sourceCode, const ShaderType type) -> unsigned int
{
    // Create shaders
    const auto glType = this->getGlShaderType(type);
    const auto shader = glCreateShader(glType);

    // Compile shaders
    const auto* const sourceCodeCStr = sourceCode.c_str();
    glShaderSource(shader, 1, &sourceCodeCStr, nullptr);
    glCompileShader(shader);

    return shader;
}

auto Toyengine::Shader::checkCompileErrors(const unsigned int object,
                                           const InfoLogType type = InfoLogType::SHADER) -> void
{
    constexpr auto errorBufferSize = 1024;
    int success;
    char infoLog[errorBufferSize];
    if (type != InfoLogType::PROGRAM)
    {
        glGetShaderiv(object, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            glGetShaderInfoLog(object, errorBufferSize, nullptr, infoLog);
            spdlog::error("Error compiling shaders: {}", infoLog);
        }
    }
    else
    {
        glGetProgramiv(object, GL_LINK_STATUS, &success);
        if (success == 0)
        {
            glGetProgramInfoLog(object, errorBufferSize, nullptr, infoLog);
            spdlog::error("Error linking shaders: {}", infoLog);
        }
    }
}

Toyengine::Shader::~Shader()
{
}

auto Toyengine::Shader::getId() const -> unsigned int
{
    return this->m_Id;
}

Toyengine::Shader::Shader(const std::string& vertexCode, const std::string& fragmentCode,
                          const std::string& geometryCode)
{
    // Setup shaders
    unsigned int geometryShader = 0;

    const auto vertexShader = this->compile(vertexCode, ShaderType::VERTEX);
    this->checkCompileErrors(vertexShader);

    const auto fragmentShader = this->compile(fragmentCode, ShaderType::FRAGMENT);
    this->checkCompileErrors(fragmentShader);
    if (!geometryCode.empty())
    {
        geometryShader = this->compile(geometryCode, ShaderType::GEOMETRY);
        this->checkCompileErrors(fragmentShader);
    }

    // Link shaders
    this->m_Id = glCreateProgram();
    glAttachShader(this->m_Id, vertexShader);
    glAttachShader(this->m_Id, fragmentShader);
    if (geometryShader != 0)
    {
        glAttachShader(this->m_Id, geometryShader);
    }
    glLinkProgram(this->m_Id);
    this->checkCompileErrors(this->m_Id, InfoLogType::PROGRAM);

    // Delete shaders
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometryShader != 0)
    {
        glDeleteShader(geometryShader);
    }
}

auto Toyengine::Shader::activate() -> void
{
    glUseProgram(this->m_Id);
}
