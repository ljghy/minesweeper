#include <minesweeper/MineMapRenderer.h>
#include <vector>
#include <utils/ResourceManager.h>

namespace minesweeper
{

void MineMapRenderer::create(MineMap &mineMap)
{
    m_pMineMap = &mineMap;

    uint16_t w = m_pMineMap->getWidth(),
             h = m_pMineMap->getHeight();
    std::vector<float> VBOData;

    uint16_t vertCount = 6 * w * h;
    float p[]{
        0, 0, 0, 0, 0,
        1, 1, 1, 1, 0,
        1, 0, 1, 0, 0,

        0, 0, 0, 0, 0,
        0, 1, 0, 1, 0,
        1, 1, 1, 1, 0};

    VBOData.resize(5 * vertCount);
    for (uint16_t i = 0; i < h; ++i)
        for (uint16_t j = 0; j < w; ++j)
        {
            uint16_t idx = 30 * (i * w + j);
            for (uint8_t k = 0; k < 30; k += 5)
            {
                VBOData[idx + k + 0] = p[k + 0] + static_cast<float>(j);
                VBOData[idx + k + 1] = p[k + 1] + static_cast<float>(i);
                VBOData[idx + k + 2] = p[k + 2];
                VBOData[idx + k + 3] = p[k + 3];
            }
        }
    m_VBO.create(VBOData.data(), 5 * vertCount * sizeof(float), GL_DYNAMIC_DRAW);
    m_VAO.create();
    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 4);
    layout.push(GL_FLOAT, 1);
    m_VAO.addBuffer(m_VBO, layout);

    m_pShader = &ResourceManager::getShader("mine_map_shader");

    FrameBufferCreateInfo fboInfo{m_viewportWidth, m_viewportHeight};
    m_FBO.create(fboInfo);

    m_mineMapTex = &ResourceManager::getTexture("mine_map_tex");

    float quadVert[]{
        -1.f, -1.f, 0.f, 0.f,
        1.f, 1.f, 1.f, 1.f,
        1.f, -1.f, 1.f, 0.f,
        -1.f, -1.f, 0.f, 0.f,
        -1.f, 1.f, 0.f, 1.f,
        1.f, 1.f, 1.f, 1.f};
    m_bkVBO.create(quadVert, sizeof(quadVert), GL_STATIC_DRAW);
    m_bkVAO.create();
    VertexBufferLayout bkLayout;
    bkLayout.push(GL_FLOAT, 4);
    m_bkVAO.addBuffer(m_bkVBO, bkLayout);

    m_pBkShader = &ResourceManager::getShader("background_shader");
    m_bkTex = &ResourceManager::getTexture("background_tex");
}

std::array<float, 4> MineMapRenderer::render(uint16_t viewportWidth, uint16_t viewportHeight)
{
    uint16_t mw = m_pMineMap->getWidth(), mh = m_pMineMap->getHeight();
    m_VBO.bind();
    void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    for (uint16_t r = 0; r < mh; ++r)
        for (uint16_t c = 0; c < mw; ++c)
        {
            auto info = m_pMineMap->getBlockInfo(r, c);
            uint16_t idx = 30 * (r * mw + c) + 4;
            for (uint16_t i = idx; i < idx + 30; i += 5)
            {
                *((float *)ptr + i) = static_cast<float>(info.state == COVERED && info.isHighlighted ? 3 : info.state);
            }
        }
    glUnmapBuffer(GL_ARRAY_BUFFER);

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
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render background

    int tw = m_bkTex->width(), th = m_bkTex->height();
    if (w * th - h * tw > 0)
    {
        view[1] = static_cast<float>(h) / th * tw / w;
        view[3] = 0.5f * (1.f - view[1]);
    }
    else
    {
        view[0] = static_cast<float>(w) / tw * th / h;
        view[2] = 0.5f * (1.f - view[0]);
    }

    m_bkVAO.bind();
    m_pBkShader->use();
    m_bkTex->bind(0);
    m_pBkShader->setUniform1i("tex", 0);
    m_pBkShader->setUniform4fv("view", view.data());
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // Render mine map

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
    m_pShader->use();
    m_pShader->setUniform4fv("view", view.data());
    m_mineMapTex->bind(0);
    m_pShader->setUniform1i("tex", 0);

    glDrawArrays(GL_TRIANGLES, 0, 6 * mw * mh);

    m_FBO.unbind();
    return view;
}

unsigned int MineMapRenderer::tex() const
{
    return m_FBO.tex().id();
}
} // namespace minesweeper
