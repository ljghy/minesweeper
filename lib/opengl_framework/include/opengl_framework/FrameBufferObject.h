#ifndef FRAME_BUFFER_OBJECT_H_
#define FRAME_BUFFER_OBJECT_H_
#include <opengl_framework/Texture2D.h>

struct FrameBufferCreateInfo
{
    int width;
    int height;
    GLenum internalFmt = GL_RGBA;
    GLenum dataType = GL_UNSIGNED_BYTE;
};

class FrameBufferObject
{
private:
    FrameBufferCreateInfo m_info;
    unsigned int m_id;
    Texture2D m_tex;

public:
    FrameBufferObject();
    ~FrameBufferObject();
    FrameBufferObject(const FrameBufferObject &) = delete;
    FrameBufferObject &operator=(const FrameBufferObject &) = delete;

    void create(const FrameBufferCreateInfo &info);
    void destroy();
    void resize(int width, int height);

    void bind() const;
    void unbind() const;

    unsigned int id() const;

    const Texture2D &tex() const;
};

#endif