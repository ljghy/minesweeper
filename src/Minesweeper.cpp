#include <iostream>

#include <imgui.h>

#include <Minesweeper.h>

#include <utils/ResourceManager.h>
namespace minesweeper
{

void Minesweeper::init(int argc, char *argv[])
{
    ResourceManager::loadShaderFromFile("../res/shaders/mine_map.shader", "mine_map_shader");
    ResourceManager::loadTextureFromFile("../res/textures/mine_map_tex_blue_alpha.png", "mine_map_tex", GL_NEAREST);

    ResourceManager::loadShaderFromFile("../res/shaders/background.shader", "background_shader");
    ResourceManager::loadTextureFromFile("../res/textures/background_tex.png", "background_tex");

    m_mineMap.init(m_difficulty.width, m_difficulty.height, m_difficulty.mineCount);
    m_mineMapRenderer.create(m_mineMap);
    m_state = GAME_PLAYING;
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

void Minesweeper::showWinWindow(const ImVec2 &center)
{
    auto io = ImGui::GetIO();
    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Minesweeper", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("Minesweeper");

    ImGui::Text("You Win!");

    ImGui::Separator();

    if (ImGui::BeginTable("Statistics", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Time");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", 0.f);
        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::Button("New game"))
    {
        m_state = GAME_INIT;
        ImGui::SetWindowFocus("Viewport");
    }

    ImGui::SameLine();
    if (ImGui::Button("Quit"))
        m_state = GAME_QUIT;

    ImGui::End();
}

void Minesweeper::showLoseWindow(const ImVec2 &center)
{
    auto io = ImGui::GetIO();
    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));

    ImGui::Begin("Minesweeper", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("Minesweeper");

    ImGui::Text("You Lose!");

    ImGui::Separator();

    if (ImGui::BeginTable("Statistics", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Time");
        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%.3f", 0.f);
        ImGui::EndTable();
    }

    ImGui::Separator();

    if (ImGui::Button("New game"))
    {
        m_state = GAME_INIT;
        ImGui::SetWindowFocus("Viewport");
    }

    ImGui::SameLine();
    if (ImGui::Button("Quit"))
        m_state = GAME_QUIT;

    ImGui::End();
}

void Minesweeper::renderGui()
{
    ImGui::SetNextWindowPos(ImGui::GetMainViewport()->Pos);
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::Begin("Viewport", nullptr,
                 ImGuiWindowFlags_NoDecoration |
                     ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoTitleBar |
                     ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_MenuBar |
                     ImGuiWindowFlags_NoDocking);

    showMenuBar();
    {
        ImGui::BeginChild("Minemap");
        ImVec2 windowSize = ImGui::GetWindowSize();
        glm::vec4 transform = m_mineMapRenderer.render(static_cast<uint16_t>(windowSize.x + 0.5f), static_cast<uint16_t>(windowSize.y + 0.5f));
        ImGui::Image(reinterpret_cast<ImTextureID>(m_mineMapRenderer.tex()), windowSize, ImVec2(0, 0), ImVec2(1, 1));

        ImVec2 windowPos = ImGui::GetWindowPos();
        ImVec2 mousePos = ImGui::GetMousePos();
        m_mineMapMouseX = static_cast<int16_t>(glm::floor((2.f * (mousePos.x - windowPos.x) / windowSize.x - 1.f - transform.z) / transform.x));
        m_mineMapMouseY = static_cast<int16_t>(glm::floor((2.f * (mousePos.y - windowPos.y) / windowSize.y - 1.f - transform.w) / transform.y));
        ImGui::EndChild();
    }

    ImVec2 windowPos = ImGui::GetWindowPos(), windowSize = ImGui::GetWindowSize();
    ImVec2 center = ImVec2(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);

    ImGui::End();
    ImGui::PopStyleVar();

    if (m_state == GAME_WIN)
        showWinWindow(center);
    else if (m_state == GAME_LOSE)
        showLoseWindow(center);
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
    if (m_state == GAME_PLAYING)
    {
        m_mineMap.update(m_mineMapMouseY, m_mineMapMouseX, getOperation());
        auto gs = m_mineMap.getGameState();

        if (gs.state == GameState::CONTINUE)
        {
            m_state = GAME_PLAYING;
        }
        else if (gs.state == GameState::LOSE)
        {
            m_state = GAME_LOSE;
        }
        else // if(gs.state == GameState::WIN)
        {
            m_state = GAME_WIN;
        }
    }
    else if (m_state == GAME_INIT)
    {
        m_mineMap.init(m_difficulty.width, m_difficulty.height, m_difficulty.mineCount);
        m_mineMapRenderer.create(m_mineMap);
        m_state = GAME_PLAYING;
    }
}

void Minesweeper::terminate()
{
}
} // namespace minesweeper