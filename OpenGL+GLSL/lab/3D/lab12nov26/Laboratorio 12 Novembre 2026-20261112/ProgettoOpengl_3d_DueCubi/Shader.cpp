#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

// Legge tutto il testo di un file (il codice GLSL).
static std::string readFile(const char* filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("File shader non trovato: ") + filename +
            ". Controllare la cartella di lavoro dell'eseguibile.");
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Compila un singolo shader e segnala eventuali errori GLSL.
static GLuint compileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* text = source.c_str();
    glShaderSource(shader, 1, &text, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
        char log[2048] = {};
        glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
        glDeleteShader(shader);
        throw std::runtime_error(std::string("Errore compilazione shader: ") + log);
    }
    return shader;
}

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    std::string vertexCode = readFile(vertexFile);
    std::string fragmentCode = readFile(fragmentFile);

    GLuint vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
    GLuint fragment = 0;
    try {
        fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
    } catch (...) {
        glDeleteShader(vertex);
        throw;
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertex);
    glAttachShader(program_, fragment);
    glLinkProgram(program_);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint success = GL_FALSE;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (success == GL_FALSE) {
        char log[2048] = {};
        glGetProgramInfoLog(program_, sizeof(log), nullptr, log);
        glDeleteProgram(program_);
        program_ = 0;
        throw std::runtime_error(std::string("Errore linking programma GLSL: ") + log);
    }
}

Shader::~Shader() { if (program_ != 0) glDeleteProgram(program_); }
void Shader::use() const { glUseProgram(program_); }
void Shader::setMatrix(const char* name, const glm::mat4& matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(program_, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVec3(const char* name, const glm::vec3& value) const {
    glUniform3fv(glGetUniformLocation(program_, name), 1, glm::value_ptr(value));
}

void Shader::setInt(const char* name, int value) const {
    glUniform1i(glGetUniformLocation(program_, name), value);
}
