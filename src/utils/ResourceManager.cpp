#include <utils/ResourceManager.h>
#include <cassert>
#include <fstream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

std::unordered_map<std::string, Shader> ResourceManager::m_shaders;
std::unordered_map<std::string, Texture2D> ResourceManager::m_textures;

void ResourceManager::loadShaderFromFile(const std::string &filename,
                                         const std::string &name)
{
    std::ifstream fs(filename);
    if (!fs)
        throw std::runtime_error("Failed to load shader from \"" + filename + "\"");
    std::string line;
    std::stringstream vss, fss;
    std::stringstream *currss = &vss;
    while (std::getline(fs, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                currss = &vss;
            else if (line.find("fragment") != std::string::npos)
                currss = &fss;
        }
        else
        {
            (*currss) << line << '\n';
        }
    }
    fs.close();

    auto p = m_shaders.insert(std::make_pair(name, Shader()));
    assert(p.second);
    p.first->second.create(vss.str(), fss.str());
}

void ResourceManager::loadTextureFromFile(const std::string &filename,
                                          const std::string &name,
                                          GLenum filterMode)
{
    Texture2DCreateInfo createInfo;
    int channels;
    stbi_uc *data = stbi_load(filename.c_str(),
                              &createInfo.width,
                              &createInfo.height,
                              &channels, 4);
    assert(data != nullptr);

    createInfo.internalFmt = GL_RGBA;
    createInfo.dataType = GL_UNSIGNED_BYTE;
    createInfo.data = data;
    createInfo.dataFmt = createInfo.internalFmt;
    createInfo.mipmapLevel = 0;
    createInfo.genMipmap = true;

    createInfo.wrapMode = GL_REPEAT;
    createInfo.filterMode = filterMode;

    auto p = m_textures.insert(std::make_pair(name, Texture2D()));
    assert(p.second);
    p.first->second.create(createInfo);
    stbi_image_free(data);
}

Shader &ResourceManager::getShader(const std::string &name)
{
    auto ite = m_shaders.find(name);
    assert(ite != m_shaders.end());
    return ite->second;
}

Texture2D &ResourceManager::getTexture(const std::string &name)
{
    auto ite = m_textures.find(name);
    assert(ite != m_textures.end());
    return ite->second;
}

void ResourceManager::cleanup()
{
    m_shaders.clear();
    m_textures.clear();
}