#include <iostream>

#include <imgui.h>

#include <Minesweeper.h>

#include <utils/ResourceManager.h>
namespace minesweeper
{

void Minesweeper::init(int argc, char *argv[])
{
    ResourceManager::loadShaderFromFile("../res/shaders/mine_map.shader", "mine_map_shader");

    ResourceManager::loadTextureFromFile("../res/textures/mine_map_tex.png", "mine_map_tex");

    m_mineMap.init(16, 16, 40);
    m_mineMapRenderer.create(m_mineMap);
}

void Minesweeper::showMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Game"))
        {
            if (ImGui::MenuItem("New"))
            {
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Options"))
        {
            if (ImGui::MenuItem("Preferences"))
            {
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void Minesweeper::renderGui()
{

    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("viewport", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_MenuBar);

    showMenuBar();
    {
        ImGui::BeginChild("minemap");
        ImVec2 windowSize = ImGui::GetWindowSize();
        glm::vec4 transform = m_mineMapRenderer.render(static_cast<uint16_t>(windowSize.x + 0.5f), static_cast<uint16_t>(windowSize.y + 0.5f));
        ImGui::Image((ImTextureID)m_mineMapRenderer.tex(), windowSize, ImVec2(0, 0), ImVec2(1, 1));

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 mousePos = ImGui::GetMousePos();
        m_mineMapMouseX = static_cast<int16_t>(glm::floor((2.f * (mousePos.x - windowPos.x) / windowSize.x - 1.f - transform.z) / transform.x));
        m_mineMapMouseY = static_cast<int16_t>(glm::floor((2.f * (mousePos.y - windowPos.y) / windowSize.y - 1.f - transform.w) / transform.y));
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

Operation Minesweeper::getOperation()
{
    if (m_mineMapMouseY < 0 || m_mineMapMouseY >= m_mineMap.getHeight() || m_mineMapMouseX < 0 || m_mineMapMouseX >= m_mineMap.getWidth())
        return HOVER;

    bool lc = ImGui::IsMouseClicked(0), rc = ImGui::IsMouseClicked(1), wc = ImGui::IsMouseClicked(2),
         ld = ImGui::IsMouseDown(0), rd = ImGui::IsMouseDown(1), wd = ImGui::IsMouseDown(2),
         lr = ImGui::IsMouseReleased(0), rr = ImGui::IsMouseReleased(1), wr = ImGui::IsMouseReleased(2);

    if (wr || (lr && rr) || (lr && rd) || (rr && ld))
        return UNCOVER_BATCH;

    if (wd || (ld && rd))
        return HIGHLIGHT_NEIGHBORS;

    if (rc && !(ld || wd))
        return MARK_BLOCK;

    static bool leftClickedOnly = false;
    if (lc)
        leftClickedOnly = true;
    if (rc || wc)
        leftClickedOnly = false;
    if (leftClickedOnly && lr)
        return UNCOVER_SINGLE;

    return HOVER;
}

void Minesweeper::update()
{
    m_mineMap.update(m_mineMapMouseY, m_mineMapMouseX, getOperation());
}

void Minesweeper::terminate()
{
}
} // namespace minesweeper