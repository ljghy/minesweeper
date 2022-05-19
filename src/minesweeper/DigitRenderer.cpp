#include <minesweeper/DigitRenderer.h>
#include <utils/ResourceManager.h>

namespace minesweeper
{

void DigitRenderer::create(uint16_t width, uint16_t height)
{
    m_width = width;
    m_height = height;

    float trd = 1.f / 3;
    float data[]{
        -1, -1, 0, 0, 0,
        -trd, 1, 1, 1, 0,
        -trd, -1, 1, 0, 0,
        -1, -1, 0, 0, 0,
        -1, 1, 0, 1, 0,
        -trd, 1, 1, 1, 0,

        -trd, -1, 0, 0, 1,
        trd, 1, 1, 1, 1,
        trd, -1, 1, 0, 1,
        -trd, -1, 0, 0, 1,
        -trd, 1, 0, 1, 1,
        trd, 1, 1, 1, 1,

        trd, -1, 0, 0, 2,
        1, 1, 1, 1, 2,
        1, -1, 1, 0, 2,
        trd, -1, 0, 0, 2,
        trd, 1, 0, 1, 2,
        1, 1, 1, 1, 2};

    m_VBO.create(data, sizeof(data), GL_STATIC_DRAW);
    m_VAO.create();
    VertexBufferLayout layout;
    layout.push(GL_FLOAT, 4);
    layout.push(GL_FLOAT, 1);
    m_VAO.addBuffer(m_VBO, layout);

    m_pShader = &ResourceManager::getShader("digit_shader");

    FrameBufferCreateInfo fboInfo{m_width, m_height};
    m_FBO.create(fboInfo);

    m_digitsTex = &ResourceManager::getTexture("digits_tex");
}

void DigitRenderer::render(int *d)
{
    m_FBO.bind();
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    m_VAO.bind();
    m_pShader->use();
    m_digitsTex->bind(0);
    m_pShader->setUniform1f("tex", 0);

    m_pShader->setUniform1i("digit[0]", d[0]);
    m_pShader->setUniform1i("digit[1]", d[1]);
    m_pShader->setUniform1i("digit[2]", d[2]);

    glDrawArrays(GL_TRIANGLES, 0, 18);

    m_FBO.unbind();
}

} // namespace minesweeper