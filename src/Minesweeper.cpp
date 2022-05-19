#include <imgui.h>

#include <Minesweeper.h>

#include <utils/ResourceManager.h>
#include <algorithm>
namespace minesweeper
{

void Minesweeper::init(int argc, char *argv[])
{
    ResourceManager::loadShaderFromFile("../res/shaders/mine_map.shader", "mine_map_shader");
    ResourceManager::loadTextureFromFile("../res/textures/mine_map_tex_blue_alpha.png", "mine_map_tex", GL_NEAREST);

    ResourceManager::loadShaderFromFile("../res/shaders/background.shader", "background_shader");
    ResourceManager::loadTextureFromFile("../res/textures/background_tex.png", "background_tex");

    ResourceManager::loadShaderFromFile("../res/shaders/digit.shader", "digit_shader");
    ResourceManager::loadTextureFromFile("../res/textures/digits_tex.png", "digits_tex", GL_NEAREST);

    m_timerIntRenderer.create(96, 60);
    m_timerDecRenderer.create(48, 30);
    m_mineCountRenderer.create(96, 60);
    initGame();
}

void Minesweeper::initGame()
{
    m_mineMap.init(m_difficulty.width, m_difficulty.height, m_difficulty.mineCount);
    m_mineMapRenderer.create(m_mineMap);
    m_state = GAME_IDLE;
    m_timer.clear();
    m_remainingMineCount = m_difficulty.mineCount;
}

void Minesweeper::showMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Game"))
        {
            if (ImGui::MenuItem("New"))
                initGame();

            ImGui::Separator();

            if (ImGui::MenuItem("Easy", NULL, m_difficulty.difficulty == EASY) && m_difficulty.difficulty != EASY)
            {
                m_difficulty = DifficultySelection(EASY);
                initGame();
            }
            if (ImGui::MenuItem("Normal", NULL, m_difficulty.difficulty == NORMAL) && m_difficulty.difficulty != NORMAL)
            {
                m_difficulty = DifficultySelection(NORMAL);
                initGame();
            }
            if (ImGui::MenuItem("Hard", NULL, m_difficulty.difficulty == HARD) && m_difficulty.difficulty != HARD)
            {
                m_difficulty = DifficultySelection(HARD);
                initGame();
            }
            if (ImGui::MenuItem("Custom", NULL, m_difficulty.difficulty == CUSTOM) && m_difficulty.difficulty != CUSTOM)
            {
                m_difficulty = DifficultySelection(CUSTOM, 10, 10, 10);
                initGame();
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Quit"))
            {
                m_state = GAME_QUIT;
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

void Minesweeper::showStatistics()
{
    if (ImGui::BeginTable("Statistics", 2))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Mode");
        ImGui::Text("Time");

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", difficultyStr[m_difficulty.difficulty]);
        ImGui::Text("%.3fs", m_timer.record());
        ImGui::EndTable();
    }
}

void Minesweeper::showFinishWindow(const ImVec2 &center)
{
    auto io = ImGui::GetIO();
    ImGui::SetNextWindowPos(center, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Minesweeper", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("Minesweeper");

    if (m_state == GAME_WIN)
        ImGui::Text("You Win!");
    else
        ImGui::Text("You Lose!");

    ImGui::Separator();

    showStatistics();

    ImGui::Separator();

    if (ImGui::Button("New game"))
        initGame();

    ImGui::SameLine();
    if (ImGui::Button("Quit"))
        m_state = GAME_QUIT;

    ImGui::End();
}

void Minesweeper::showTimer()
{
    float t = m_timer.query();
    int d[6];
    assert(t >= 0.f);
    if (t > 999.999f || t < 0.f)
        std::fill(d, d + 6, 9);
    else
    {
        uint16_t a = static_cast<uint16_t>(t);
        d[0] = a / 100;
        d[1] = a / 10 % 10;
        d[2] = a % 10;
        a = static_cast<uint16_t>((t - a) * 1000.f + 0.5f) % 1000;
        d[3] = a / 100;
        d[4] = a / 10 % 10;
        d[5] = a % 10;
    }
    m_timerIntRenderer.render(d);
    m_timerDecRenderer.render(d + 3);
    ImGui::Image((ImTextureID)m_timerIntRenderer.tex(), ImVec2(m_timerIntRenderer.width(), m_timerIntRenderer.height()));
    ImGui::SameLine();
    ImGui::Image((ImTextureID)m_timerDecRenderer.tex(), ImVec2(m_timerDecRenderer.width(), m_timerDecRenderer.height()));
    ImGui::SameLine();
}

void Minesweeper::showRemainingMineCount()
{
    int c = m_remainingMineCount;
    int d[3];
    d[0] = c < 0 ? 10 : 0;
    c = std::min(std::abs(c), 99);
    d[1] = c / 10;
    d[2] = c % 10;
    m_mineCountRenderer.render(d);
    ImGui::Image((ImTextureID)m_mineCountRenderer.tex(), ImVec2(m_mineCountRenderer.width(), m_mineCountRenderer.height()));
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
    showTimer();
    showRemainingMineCount();

    ImGui::Separator();

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

    if (m_state == GAME_WIN || m_state == GAME_LOSE)
        showFinishWindow(center);
}

Operation Minesweeper::getOperation()
{
    if (m_mineMapMouseY < 0 || m_mineMapMouseY >= m_mineMap.getHeight() || m_mineMapMouseX < 0 || m_mineMapMouseX >= m_mineMap.getWidth())
        return HOVER;

    bool lc = ImGui::IsMouseClicked(0), rc = ImGui::IsMouseClicked(1), wc = ImGui::IsMouseClicked(2),
         ld = ImGui::IsMouseDown(0), rd = ImGui::IsMouseDown(1), wd = ImGui::IsMouseDown(2),
         lr = ImGui::IsMouseReleased(0), rr = ImGui::IsMouseReleased(1), wr = ImGui::IsMouseReleased(2);

    static bool leftClickedOnly = false;
    if (lc)
        leftClickedOnly = true;
    if (rc || wc || rd || wd || rr || wr)
        leftClickedOnly = false;

    if (wr || (lr && rr) || (lr && rd) || (rr && ld))
        return UNCOVER_BATCH;

    if (wd || (ld && rd))
        return HIGHLIGHT_NEIGHBORS;

    if (rc && !(ld || wd))
        return MARK_BLOCK;

    if (leftClickedOnly && lr)
    {
        leftClickedOnly = false;
        return UNCOVER_SINGLE;
    }

    return HOVER;
}

void Minesweeper::update()
{
    if (m_state == GAME_PLAYING || m_state == GAME_IDLE)
    {
        auto oprt = getOperation();
        if (m_mineMapMouseX >= 0 && m_mineMapMouseX < m_mineMap.getWidth() && m_mineMapMouseY >= 0 && m_mineMapMouseY < m_mineMap.getHeight())
        {
            if (m_state == GAME_IDLE && oprt == UNCOVER_SINGLE && m_mineMap.getBlockInfo(m_mineMapMouseY, m_mineMapMouseX).state == COVERED)
            {
                m_timer.start();
                m_mineMap.init(m_difficulty.width, m_difficulty.height, m_difficulty.mineCount, m_mineMapMouseX, m_mineMapMouseY);
                m_state = GAME_PLAYING;
            }
        }
        m_mineMap.update(m_mineMapMouseY, m_mineMapMouseX, oprt);
        auto gs = m_mineMap.getGameState();
        m_remainingMineCount = gs.remainingMineCount;
        if (gs.state == GameState::LOSE)
        {
            m_state = GAME_LOSE;
            m_timer.stop();
        }
        else if (gs.state == GameState::WIN)
        {
            m_state = GAME_WIN;
            m_timer.stop();
        }
    }
}

void Minesweeper::terminate()
{
}
} // namespace minesweeper