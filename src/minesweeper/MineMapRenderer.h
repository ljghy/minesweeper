#ifndef MINE_MAP_RENDERER_H_
#define MINE_MAP_RENDERER_H_

#include "MineMap.h"
#include <opengl_framework/FrameBufferObject.h>
#include <opengl_framework/VertexBufferObject.h>
#include <opengl_framework/VertexArrayObject.h>
#include <array>

namespace minesweeper
{
class MineMapRenderer
{
private:
    MineMap *m_pMineMap;

    FrameBufferObject m_FBO;
    VertexBufferObject m_VBO;
    VertexArrayObject m_VAO;

    uint16_t m_mineMapWidth;
    uint16_t m_mineMapHeight;
    uint16_t m_viewportWidth;
    uint16_t m_viewportHeight;
    std::array<float, 4> m_mineMapTransform;

    std::vector<float> m_texId;

public:
    MineMapRenderer();
    void create(MineMap &mineMap);
    std::array<float, 4> render(uint16_t viewportWidth, uint16_t viewportHeight);
    unsigned int tex() const;
};
}; // namespace minesweeper

#endif