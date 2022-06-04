#include "MineMapRenderer.h"
#include "ResourceManager.h"
#include "PreferencesManager.h"
#include <vector>

namespace minesweeper
{
MineMapRenderer::MineMapRenderer()
    : m_pMineMap(nullptr), m_viewportWidth(0), m_viewportHeight(0)
{
}

void MineMapRenderer::create(MineMap &mineMap)
{
    m_pMineMap = &mineMap;
    uint16_t w = mineMap.getWidth(),
             h = mineMap.getHeight();

    if (m_mineMapWidth != w || m_mineMapHeight != h)
    {
        m_mineMapWidth = w;
        m_mineMapHeight = h;

        uint16_t vertCount = 6 * w * h;
        m_VBO.create(nullptr, vertCount * sizeof(float), GL_DYNAMIC_DRAW);
        m_VAO.create();
        VertexBufferLayout layout;
        layout.push(GL_FLOAT, 1);
        m_VAO.addBuffer(m_VBO, layout);
    }
}

std::array<float, 4> MineMapRenderer::render(uint16_t viewportWidth, uint16_t viewportHeight)
{
    uint16_t mw = m_pMineMap->getWidth(), mh = m_pMineMap->getHeight();

    uint16_t vertCount = 6 * mw * mh;
    m_texId.resize(vertCount);
    for (uint16_t r = 0; r < mh; ++r)
        for (uint16_t c = 0; c < mw; ++c)
        {
            auto info = m_pMineMap->getBlockInfo(r, c);
            uint16_t idx = 6 * (r * mw + c);
            float texId = static_cast<float>(info.state == COVERED && info.isHighlighted ? 3 : info.state);
            for (uint16_t i = idx; i < idx + 6; ++i)
                m_texId[i] = texId;
        }
    m_VBO.bind();
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * vertCount, m_texId.data());

    if (m_viewportWidth != viewportWidth || m_viewportHeight != viewportHeight)
    {
        m_FBO.resize(viewportWidth, viewportHeight);
        m_viewportWidth = viewportWidth;
        m_viewportHeight = viewportHeight;
    }

    auto &w = viewportWidth;
    auto &h = viewportHeight;
    std::array<float, 4> view{1.f, 1.f, 0.f, 0.f};

    m_FBO.bind();
    glViewport(0, 0, w, h);

    auto &bkColor = PreferencesManager::preferences.backgroundColor;
    glClearColor(bkColor[0], bkColor[1], bkColor[2], bkColor[3]);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);

    float iaspect = static_cast<float>(h) / static_cast<float>(w);
    if (w * mh - h * mw > 0)
    {
        view[0] = 2.f / mh * iaspect;
        view[1] = 2.f / mh;
        view[2] = -view[0] * mw * 0.5f;
        view[3] = -1.f;
    }
    else
    {
        view[0] = 2.f / mw;
        view[1] = 2.f / mw / iaspect;
        view[2] = -1.f;
        view[3] = -view[1] * mh * 0.5f;
    }

    m_VAO.bind();

    Shader *shader = ResourceManager::getShader("mine_map_shader");
    shader->use();
    shader->setUniform4fv("view", view.data());
    ResourceManager::getTexture("mine_map_tex")->bind(0);
    shader->setUniform1i("tex", 0);
    shader->setUniform1i("width", static_cast<int>(mw));
    shader->setUniform1f("opacity", PreferencesManager::preferences.mineMapOpacity);

    glDrawArrays(GL_TRIANGLES, 0, 6 * mw * mh);

    m_FBO.unbind();
    return view;
}

unsigned int MineMapRenderer::tex() const
{
    return m_FBO.tex().id();
}
} // namespace minesweeper
