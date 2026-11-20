#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
public:
    // I parametri sono percorsi di file GLSL, non il loro contenuto.
    Shader(const char* vertexFile, const char* fragmentFile);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;
    void setMatrix(const char* name, const glm::mat4& matrix) const;
    void setVec3(const char* name, const glm::vec3& value) const;
    void setInt(const char* name, int value) const;

private:
    GLuint program_ = 0;
};
