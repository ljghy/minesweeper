#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <unordered_map>
#include <string>
#include <opengl_framework/Shader.h>
#include <opengl_framework/Texture2D.h>
class ResourceManager
{
private:
    static std::unordered_map<std::string, Shader> m_shaders;
    static std::unordered_map<std::string, Texture2D> m_textures;

public:
    static void loadShaderFromFile(const std::string &filename, const std::string &name, bool optional = false);
    static void loadTextureFromFile(const std::string &filename, const std::string &name, GLenum filterMode = GL_LINEAR, bool optional = false);

    static Shader *getShader(const std::string &name);
    static Texture2D *getTexture(const std::string &name);

    static void cleanup();
};

#endif
