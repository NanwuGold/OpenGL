#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const char* vertex_path, const char* fragment_path, const char* geometry_path)
{
    // 读取 着色器的源码文件
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    std::ifstream gShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开文件
        vShaderFile.open(vertex_path);
        fShaderFile.open(fragment_path);

        // 创建字符串流对象 -- 存储文件中的数据
        std::stringstream vShaderStream, fShaderStream;

        // 将文件读入到流中
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // 关闭文件
        vShaderFile.close();
        fShaderFile.close();

        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        if (geometry_path != nullptr)
        {
            gShaderFile.open(geometry_path);
            std::stringstream geometry_stream;
            geometry_stream << gShaderFile.rdbuf();
            gShaderFile.close();
            geometryCode = geometry_stream.str();
        }
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "ERROR::SHADER:FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }

    // 获取C风格的字符串
    const char* vertexShaderSource = vertexCode.c_str();
    const char* fragmentShaderSource = fragmentCode.c_str();

    // compile  OGLShader  -- 编译着色器
    unsigned int vertex, fragment;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
 glCompileShader(vertex);
    checkCompileErrors(vertex, shaderType::vertexShader);

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, shaderType::fragmentShader);

    unsigned int geometry;
    if (geometry_path != nullptr)
    {
        const char* geometryShaderSource = geometryCode.c_str();
        geometry = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometry, 1, &geometryShaderSource, nullptr);
        glCompileShader(geometry);
        checkCompileErrors(geometry, shaderType::geometryShader);
    }

    // 着色器程序
    m_program = glCreateProgram();
    glAttachShader(m_program, vertex);
    glAttachShader(m_program, fragment);

    if (geometry_path)
    {
        glAttachShader(m_program, geometry);
    }

    glLinkProgram(m_program);
    checkCompileErrors(m_program, shaderType::programShader);

    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(m_program);
}

[[maybe_unused]] void Shader::Bind() const
{
    glUseProgram(m_program);
}

[[maybe_unused]] void Shader::setBool(const std::string& name, const bool value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(location, static_cast<int>(value));
}

[[maybe_unused]] void Shader::setInt(const std::string& name, const int value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform1i(location, value);
}

[[maybe_unused]] void Shader::setFloat(const std::string& name, const float value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform1f(location, value);
}

[[maybe_unused]] void Shader::setVec2(const std::string& name, const glm::vec2 &value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform2fv(location, 1, &value[0]);
}

[[maybe_unused]] void Shader::setVec2(const std::string& name, const float x, const float y) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform2f(location, x, y);
}

[[maybe_unused]] void Shader::setVec3(const std::string& name, glm::vec3 & value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform3fv(location, 1, &value[0]);
}

[[maybe_unused]] void Shader::setVec3(const std::string& name, const float x, const float y, const float z) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform3f(location, x, y, z);
}

[[maybe_unused]] void Shader::setVec4(const std::string& name, glm::vec4& value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform4fv(location, 1, &value[0]);
}

[[maybe_unused]] void Shader::setVec4(const std::string& name, const float x, const float y, const float z, const float w) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniform4f(location, x, y, z, w);
}

[[maybe_unused]] void Shader::setMat2(const std::string& name, glm::mat2& value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix2fv(location, 1, GL_FALSE, &value[0][0]);
}

[[maybe_unused]] void Shader::setMat3(const std::string& name, glm::mat3& value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat4(const std::string& name, glm::mat4& value) const
{
    const auto location = glGetUniformLocation(m_program, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::checkCompileErrors(const unsigned shader, const shaderType type)
{
    int success{1};
    char infoLog[1024];
    switch (type)
    {
    case shaderType::vertexShader:
    case shaderType::fragmentShader:
    case shaderType::geometryShader:
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::SHADER::COMPILATION_FAILED of type: " << (type == shaderType::vertexShader ? "VERTEX" : (type == shaderType::fragmentShader ? "FRAGMENT" : "GEOMETRY"))
                      << "\n" << infoLog << std::endl;
        }
        break;
    case shaderType::programShader:
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cout << "ERROR::PROGRAM::LINK_FAILED of type: " << "PROGRAM" << "\n" << infoLog << std::endl;
        }
        break;
    }
}
