#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>
#include "Enum.h"
#include <typeinfo>

using namespace shader;

class Shader
{
public:
    unsigned int m_program;
    Shader(const char* vertex_path, const char* fragment_path,const char * geometry_path = nullptr);
    ~Shader();
    void use() const;

    [[maybe_unused]] void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name,  int value ) const;
    void setFloat(const std::string& name,  float value ) const;
    void setVec2(const std::string& name, glm::vec2 & value) const;
    void setVec2(const std::string& name,float x, float y) const;

    void setVec3(const std::string& name, glm::vec3& value) const;
    void setVec3(const std::string& name, float x, float y,float z) const;

    [[maybe_unused]] void setVec4(const std::string& name, glm::vec4& value) const;

    [[maybe_unused]] void setVec4(const std::string& name, float x, float y,float z,float w) const;

    [[maybe_unused]] void setMat2(const std::string& name, [[maybe_unused]] glm::mat2& value) const;

    [[maybe_unused]] void setMat3(const std::string& name, glm::mat3& value) const;
    void setMat4(const std::string& name, glm::mat4& value) const;

    template<typename T1 = std::string, typename  T2>
    [[maybe_unused]] void setUniformValue(T1 & name,T2 & value)
    {
        [[maybe_unused]] const auto location = glGetUniformLocation(m_program, name.c_str());
        if constexpr (typeid(value).name() == typeid(glm::mat4).name())
        {
            // TODO
        }
    }

private:
    static void checkCompileErrors(unsigned int shader, shader_type type);

};

#endif
