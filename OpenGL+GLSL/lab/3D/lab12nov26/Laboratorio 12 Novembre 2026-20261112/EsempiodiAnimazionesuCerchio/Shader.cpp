#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>

static std::string readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) throw std::runtime_error("File shader non trovato: " + filename);
    std::ostringstream text;
    text << file.rdbuf();
    return text.str();
}

static GLuint compile(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* code = source.c_str();
    glShaderSource(shader, 1, &code, nullptr);
    glCompileShader(shader);
    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char message[2048];
        glGetShaderInfoLog(shader, sizeof(message), nullptr, message);
        glDeleteShader(shader);
        throw std::runtime_error(message);
    }
    return shader;
}

Shader::Shader(const std::string& vertexFile, const std::string& fragmentFile) {
    GLuint vertex = compile(GL_VERTEX_SHADER, readFile(vertexFile));
    GLuint fragment = 0;
    try { fragment = compile(GL_FRAGMENT_SHADER, readFile(fragmentFile)); }
    catch (...) { glDeleteShader(vertex); throw; }
    program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
    GLint ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char message[2048];
        glGetProgramInfoLog(program, sizeof(message), nullptr, message);
        glDeleteProgram(program);
        program = 0;
        throw std::runtime_error(message);
    }
}
Shader::~Shader() { if (program) glDeleteProgram(program); }
void Shader::use() const { glUseProgram(program); }
void Shader::setMatrix(const char* name, const glm::mat4& matrix) const {
    GLint location = glGetUniformLocation(program, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
}
void Shader::setFloat(const char* name, float value) const {
    glUniform1f(glGetUniformLocation(program, name), value);
}
