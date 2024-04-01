#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include "RenderBase/Render/Enum.h"
#include <typeinfo>

using namespace OGLShader;

class OpenGLShader
{
public:
    unsigned int m_program;
    OpenGLShader(const char* vertex_path, const char* fragment_path, const char * geometry_path = nullptr);
    ~OpenGLShader();
    [[maybe_unused]] void Bind() const;

    [[maybe_unused]] void setBool(const std::string& name, bool value) const;

    [[maybe_unused]] void setInt(const std::string& name,  int value ) const;

    [[maybe_unused]] void setFloat(const std::string& name,  float value ) const;

    [[maybe_unused]] void setVec2(const std::string& name, const glm::vec2 &value) const;

    [[maybe_unused]] void setVec2(const std::string& name,float x, float y) const;

    [[maybe_unused]] void setVec3(const std::string& name, const glm::vec3 &value) const;

    [[maybe_unused]] void setVec3(const std::string& name, float x, float y,float z) const;

    [[maybe_unused]] void setVec4(const std::string& name, const glm::vec4 &value) const;

    [[maybe_unused]] void setVec4(const std::string& name, float x, float y,float z,float w) const;

    [[maybe_unused]] void setMat2(const std::string& name, glm::mat2& value) const;

    [[maybe_unused]] void setMat3(const std::string& name, glm::mat3& value) const;

    [[maybe_unused]]  void setMat4(const std::string& name, const glm::mat4 &value) const;

private:
    static void checkCompileErrors(unsigned int shader, shaderType type);

};

#endif
