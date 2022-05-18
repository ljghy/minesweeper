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
    VertexBufferObject m_VBO;
    VertexArrayObject m_VAO;
    FrameBufferObject m_FBO;
    Shader *m_pShader;

    uint16_t m_viewportWidth;
    uint16_t m_viewportHeight;

    Texture2D *m_mineMapTex;

public:
    MineMapRenderer() = default;
    void create(MineMap &mineMap);
    glm::vec4 render(uint16_t viewportWidth, uint16_t viewportHeight);
    unsigned int tex() const;
};
}; // namespace minesweeper

#endif