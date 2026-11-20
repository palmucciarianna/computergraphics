#pragma once
#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <string>

class Shader {
public:
    Shader(const std::string& vertexFile, const std::string& fragmentFile);
    ~Shader();
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    void use() const;
    void setMatrix(const char* name, const glm::mat4& matrix) const;
    void setFloat(const char* name, float value) const;
private:
    GLuint program = 0;
};
