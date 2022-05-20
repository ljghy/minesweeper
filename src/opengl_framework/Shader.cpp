#include <glad/glad.h>
#include <opengl_framework/Shader.h>
#include <stdexcept>

Shader::Shader() : m_id(0) {}

Shader::Shader(Shader &&other) : m_id(other.m_id) { other.m_id = 0; }

void Shader::create(const std::string &vertSrc, const std::string &fragSrc)
{
    destroy();
    const char *vertSrc_c = vertSrc.c_str(), *fragSrc_c = fragSrc.c_str();

    unsigned int vert, frag;
    int success;
    char infoLog[512];

    vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &vertSrc_c, NULL);
    glCompileShader(vert);
    glGetShaderiv(vert, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vert, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Vertex shader compile error: ") + infoLog);
    };

    frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &fragSrc_c, NULL);
    glCompileShader(frag);
    glGetShaderiv(frag, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(frag, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Fragment shader compile error: ") + infoLog);
    };

    m_id = glCreateProgram();
    glAttachShader(m_id, vert);
    glAttachShader(m_id, frag);
    glLinkProgram(m_id);

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        throw std::runtime_error(std::string("Shader link error: ") + infoLog);
    }

    glDeleteShader(vert);
    glDeleteShader(frag);
}

void Shader::use() const { glUseProgram(m_id); }
unsigned int Shader::getId() const { return m_id; }
void Shader::destroy()
{
    if (m_id != 0)
    {
        glDeleteProgram(m_id);
        m_id = 0;
    }
}
Shader::~Shader() { destroy(); }

void Shader::setUniform1i(const char *name, int n) const
{
    glUniform1i(glGetUniformLocation(m_id, name), n);
}

void Shader::setUniform3fv(const char *name, float *v) const
{
    glUniform3fv(glGetUniformLocation(m_id, name), 1, v);
}

void Shader::setUniform1f(const char *name, float f) const
{
    glUniform1f(glGetUniformLocation(m_id, name), f);
}

void Shader::setUniform4fv(const char *name, float *v) const
{
    glUniform4fv(glGetUniformLocation(m_id, name), 1, v);
}