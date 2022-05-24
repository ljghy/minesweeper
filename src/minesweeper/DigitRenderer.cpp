#include <minesweeper/DigitRenderer.h>
#include <utils/ResourceManager.h>

namespace minesweeper
{
void DigitRenderer::create(uint16_t width, uint16_t height)
{
    m_width = width;
    m_height = height;

    FrameBufferCreateInfo fboInfo{m_width, m_height};
    m_FBO.create(fboInfo);
}

void DigitRenderer::render(int *d)
{
    m_FBO.bind();
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    auto shader = ResourceManager::getShader("digit_shader");
    shader->use();
    ResourceManager::getTexture("digits_tex")->bind(0);
    shader->setUniform1f("tex", 0);

    shader->setUniform1i("digit[0]", d[0]);
    shader->setUniform1i("digit[1]", d[1]);
    shader->setUniform1i("digit[2]", d[2]);

    glDrawArrays(GL_TRIANGLES, 0, 18);

    m_FBO.unbind();
}

} // namespace minesweeper