#include <glad/glad.h>
#include <opengl_framework/FrameBufferObject.h>
FrameBufferObject::FrameBufferObject()
    : m_id(0)
{
}
FrameBufferObject::~FrameBufferObject() { destroy(); }

void FrameBufferObject::create(const FrameBufferCreateInfo &info)
{
    destroy();
    m_info = info;
    glGenFramebuffers(1, &m_id);
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);

    Texture2DCreateInfo texInfo;
    texInfo.width = info.width;
    texInfo.height = info.height;
    texInfo.internalFmt = info.internalFmt;
    texInfo.dataType = info.dataType;

    m_tex.create(texInfo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex.id(), 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::resize(int width, int height)
{
    destroy();
    m_info.width = width;
    m_info.height = height;
    create(m_info);
}

void FrameBufferObject::destroy()
{
    m_tex.destroy();
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        m_id = 0;
    }
}

void FrameBufferObject::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void FrameBufferObject::unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const Texture2D &FrameBufferObject::tex() const
{
    return m_tex;
}

unsigned int FrameBufferObject::id() const
{
    return m_id;
}