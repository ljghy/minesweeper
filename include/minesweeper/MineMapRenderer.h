#ifndef MINE_MAP_RENDERER_H_
#define MINE_MAP_RENDERER_H_

#include <minesweeper/MineMap.h>
#include <opengl_framework/common.h>
#include <array>

namespace minesweeper
{
class MineMapRenderer
{
private:
    MineMap *m_pMineMap;

    FrameBufferObject m_FBO;

    VertexBufferObject m_bkVBO;
    VertexArrayObject m_bkVAO;
    Shader *m_pBkShader;
    Texture2D *m_bkTex;

    VertexBufferObject m_VBO;
    VertexArrayObject m_VAO;
    Shader *m_pShader;
    Texture2D *m_mineMapTex;

    uint16_t m_viewportWidth;
    uint16_t m_viewportHeight;

public:
    MineMapRenderer() : m_viewportWidth(640), m_viewportHeight(480) {}
    void create(MineMap &mineMap);
    std::array<float, 4> render(uint16_t viewportWidth, uint16_t viewportHeight);
    unsigned int tex() const;
};
}; // namespace minesweeper

#endif