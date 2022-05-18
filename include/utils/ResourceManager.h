#ifndef RESOURCE_MANAGER_H_
#define RESOURCE_MANAGER_H_

#include <unordered_map>
#include <string>
#include <opengl_framework/common.h>
class ResourceManager
{
private:
    static std::unordered_map<std::string, Shader> m_shaders;
    static std::unordered_map<std::string, Texture2D> m_textures;

public:
    static void loadShaderFromFile(const std::string &filename, const std::string &name);
    static void loadTextureFromFile(const std::string &filename, const std::string &name);

    static Shader &getShader(const std::string &name);
    static Texture2D &getTexture(const std::string &name);
};

#endif
