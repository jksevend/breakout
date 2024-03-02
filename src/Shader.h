#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

namespace Toyengine
{
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT,
        GEOMETRY
    };

    class Shader
    {
        enum class InfoLogType
        {
            SHADER,
            PROGRAM
        };

        unsigned int m_Id;

        auto getGlShaderType(ShaderType type) -> unsigned int;
        auto compile(const std::string& sourceCode, ShaderType type) -> unsigned int;
        auto checkCompileErrors(unsigned int object, InfoLogType type) -> void;

    public:
        Shader(const std::string& vertexCode, const std::string& fragmentCode, const std::string& geometryCode = "");
        ~Shader();

        [[nodiscard]] auto getId() const -> unsigned int;

        auto activate() const -> void;

        auto setVector3f(const std::string& name, const glm::vec3& value, bool useShader = false) const -> void;
        auto setMatrix4(const std::string& name, const glm::mat4& matrix, bool useShader = false) const -> void;
        auto setInteger(const std::string& name, int i, bool useShader = false) const -> void;
    };
} // Toyengine
