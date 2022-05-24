#ifndef DIGIT_RENDERER_H_
#define DIGIT_RENDERER_H_

#include <opengl_framework/common.h>
#include <vector>

namespace minesweeper
{
class DigitRenderer
{
private:
    FrameBufferObject m_FBO;

    uint16_t m_width;
    uint16_t m_height;

public:
    DigitRenderer() = default;
    void create(uint16_t width, uint16_t height);
    void render(int *d);
    unsigned int tex() const { return m_FBO.tex().id(); }

    uint16_t width() const { return m_width; }
    uint16_t height() const { return m_height; }
};
}; // namespace minesweeper

#endif
