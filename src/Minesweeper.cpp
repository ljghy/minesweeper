#include <imgui.h>

#include <Minesweeper.h>

#include <utils/ResourceManager.h>
#include <minesweeper/RecordManager.h>

#include <algorithm>
#include <cmath>
namespace minesweeper
{
Minesweeper::Minesweeper() : m_recordRank(-1) {}

void Minesweeper::init(int argc, char *argv[])
{
    ResourceManager::loadShaderFromFile("../res/shaders/mine_map.shader", "mine_map_shader");
    ResourceManager::loadTextureFromFile("../res/textures/mine_map_tex_blue_alpha.png", "mine_map_tex", GL_NEAREST);

    ResourceManager::loadShaderFromFile("../res/shaders/background.shader", "background_shader");
    ResourceManager::loadTextureFromFile("../res/textures/background_tex.png", "background_tex");

    ResourceManager::loadShaderFromFile("../res/shaders/digit.shader", "digit_shader");
    ResourceManager::loadTextureFromFile("../res/textures/digits_tex.png", "digits_tex", GL_NEAREST);

    ResourceManager::loadTextureFromFile("../res/textures/win_face_tex.png", "win_face_tex");
    ResourceManager::loadTextureFromFile("../res/textures/lose_face_tex.png", "lose_face_tex");

    RecordManager::init();
    RecordManager::load();

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

void Minesweeper::showCustomEditor()
{
    ImGui::SetNextWindowPos(m_viewportCenter, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Custom Editor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("Custom Editor");

    static int customWidth = 8, customHeight = 8, customMineCount = 10;

    ImGui::SliderInt("width", &customWidth, 8, 30);
    ImGui::SliderInt("height", &customHeight, 8, 30);
    ImGui::SliderInt("mines", &customMineCount, 1, customWidth * customHeight / 2);

    ImGui::Separator();
    if (ImGui::Button("Apply"))
    {
        m_difficulty = DifficultySelection(CUSTOM, customWidth, customHeight, customMineCount);
        initGame();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel"))
        initGame();

    ImGui::End();
}

void Minesweeper::showRecordsWindow()
{
    ImGui::SetNextWindowPos(m_viewportCenter, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Records", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("Records");

    static int difficulty = 0;
    ImGui::RadioButton("Easy", &difficulty, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Normal", &difficulty, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Hard", &difficulty, 2);

    ImGui::Separator();

    if (ImGui::BeginTable("Records", 3))
    {
        for (auto &rec : RecordManager::records[difficulty])
        {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", rec.playerID.data());

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.3fs", rec.time);

            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", rec.timeStamp.data());
        }
        ImGui::EndTable();
    }

    ImGui::Separator();
    if (ImGui::Button("OK"))
        m_state = m_prevState;

    ImGui::End();
}

void Minesweeper::showPlayerIDEditor()
{
    ImGui::SetNextWindowPos(m_viewportCenter, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("##PlayerIDEditor", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("##PlayerIDEditor");

    ImGui::Text("New Record in %s Mode!", difficultyStr[m_difficulty.difficulty]);
    ImGui::Text("Enter your name:");
    struct PlayerIDFilter
    {
        static int filter(ImGuiInputTextCallbackData *data)
        {
            // a-z, A-Z, 0-9, _
            auto &wc = data->EventChar;
            if (wc > 127)
                return 1;
            auto c = (char)wc;
            if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || ((c >= '0') && (c <= '9')) || (c == '_'))
                return 0;
            return 1;
        }
    };

    static char buffer[playerIDMaxLen];
    ImGui::InputText("##PlayerID", buffer, playerIDMaxLen, ImGuiInputTextFlags_CallbackCharFilter, PlayerIDFilter::filter);
    ImGui::Separator();
    if (ImGui::Button("OK"))
    {
        m_recordRank = RecordManager::insertRecord(buffer, m_difficulty.difficulty, m_recordTime);
        RecordManager::write();
        m_state = GAME_WIN;
    }

    ImGui::End();
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
            if (ImGui::MenuItem("Custom", NULL, m_difficulty.difficulty == CUSTOM))
            {
                m_state = GAME_EDIT_CUSTOM;
            }

            ImGui::Separator();
            if (ImGui::MenuItem("Records"))
            {
                m_prevState = m_state;
                m_state = GAME_SHOW_RECORDS_WINDOW;
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
            if (m_mineMap.isQuestionMarkEnabled())
            {
                if (ImGui::MenuItem("Disable [?]"))
                    m_mineMap.disableQuestionMark();
            }
            else
            {
                if (ImGui::MenuItem("Enable [?]"))
                    m_mineMap.enableQuestionMark();
            }
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
    if (ImGui::BeginTable("Statistics", 3))
    {
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::Text("Mode");
        ImGui::Text("Time");

        ImGui::TableSetColumnIndex(1);
        ImGui::Text("%s", difficultyStr[m_difficulty.difficulty]);
        ImGui::Text("%.3fs", m_timer.record());
        ImGui::EndTable();
        ImGui::Separator();
    }
}

void Minesweeper::showRecords()
{
    if (m_difficulty.difficulty != CUSTOM && m_recordRank < recordCount)
    {
        if (ImGui::BeginTable("Records", 3))
        {
            auto i = m_recordRank;
            for (auto &rec : RecordManager::records[m_difficulty.difficulty])
            {
                if (i == 0 || rec.playerID == RecordManager::records[m_difficulty.difficulty][m_recordRank].playerID)
                {
                    ImVec4 color = i == 0 ? ImVec4(0.2f, 0.8f, 0.3f, 1.0f) : ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextColored(color, "%s", rec.playerID.data());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextColored(color, "%.3fs", rec.time);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextColored(color, "%s", rec.timeStamp.data());
                }
                else
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%s", rec.playerID.data());

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%.3fs", rec.time);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%s", rec.timeStamp.data());
                }
                --i;
            }
            ImGui::EndTable();
        }
        ImGui::Separator();
    }
}

void Minesweeper::showFinishWindow()
{
    ImGui::SetNextWindowPos(m_viewportCenter, ImGuiCond_Once, ImVec2(0.5f, 0.5f));
    ImGui::Begin("Minesweeper", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking);
    ImGui::SetWindowFocus("Minesweeper");

    if (m_state == GAME_WIN)
    {
        ImGui::Image((ImTextureID)ResourceManager::getTexture("win_face_tex").id(), ImVec2(34, 34));
        ImGui::SameLine();
        ImGui::Text("You Win!");
    }
    else
    {
        ImGui::Image((ImTextureID)ResourceManager::getTexture("lose_face_tex").id(), ImVec2(34, 34));
        ImGui::SameLine();
        ImGui::Text("You Lose!");
    }

    ImGui::Separator();

    showStatistics();

    showRecords();

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

    float aspect = float(m_timerIntRenderer.width()) / m_timerIntRenderer.height();
    const float r = 0.3f;
    const float maxWidth = static_cast<float>(m_timerIntRenderer.width() * 2);
    float w = maxWidth * (1.f - std::exp(-ImGui::GetWindowSize().x * r / maxWidth));

    ImGui::Image((ImTextureID)m_timerIntRenderer.tex(), ImVec2(w, w / aspect));
    ImGui::SameLine();
    w *= 0.5f;
    ImGui::Image((ImTextureID)m_timerDecRenderer.tex(), ImVec2(w, w / aspect));
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

    const float r = 0.3f;
    const float maxWidth = static_cast<float>(m_mineCountRenderer.width() * 2);
    float w = maxWidth * (1.f - std::exp(-ImGui::GetWindowSize().x * r / maxWidth));
    float aspect = float(m_mineCountRenderer.width()) / m_mineCountRenderer.height();

    ImGui::SameLine(ImGui::GetWindowSize().x - w - 10);
    ImGui::Image((ImTextureID)m_mineCountRenderer.tex(), ImVec2(w, w / aspect));
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

    ImVec2 windowPos = ImGui::GetWindowPos();
    ImVec2 windowSize = ImGui::GetWindowSize();
    m_viewportCenter = ImVec2(windowPos.x + windowSize.x * 0.5f, windowPos.y + windowSize.y * 0.5f);

    showMenuBar();
    showTimer();
    showRemainingMineCount();

    ImGui::Separator();

    {

        ImGui::BeginChild("Minemap");
        windowPos = ImGui::GetWindowPos();
        windowSize = ImGui::GetWindowSize();
        auto transform = m_mineMapRenderer.render(static_cast<uint16_t>(windowSize.x + 0.5f), static_cast<uint16_t>(windowSize.y + 0.5f));
        ImGui::Image(reinterpret_cast<ImTextureID>(m_mineMapRenderer.tex()), windowSize, ImVec2(0, 0), ImVec2(1, 1));

        ImVec2 mousePos = ImGui::GetMousePos();
        m_mineMapMouseX = static_cast<int16_t>(std::floor((2.f * (mousePos.x - windowPos.x) / windowSize.x - 1.f - transform[2]) / transform[0]));
        m_mineMapMouseY = static_cast<int16_t>(std::floor((2.f * (mousePos.y - windowPos.y) / windowSize.y - 1.f - transform[3]) / transform[1]));
        ImGui::EndChild();
    }

    ImGui::End();
    ImGui::PopStyleVar();

    if (m_state == GAME_WIN || m_state == GAME_LOSE)
        showFinishWindow();

    if (m_state == GAME_EDIT_CUSTOM)
        showCustomEditor();

    if (m_state == GAME_EDIT_PLAYER_ID)
        showPlayerIDEditor();

    if (m_state == GAME_SHOW_RECORDS_WINDOW)
        showRecordsWindow();
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
        m_recordRank = -1;
        if (gs.state == GameState::LOSE)
        {
            m_state = GAME_LOSE;
            m_timer.stop();
        }
        else if (gs.state == GameState::WIN)
        {
            m_state = GAME_WIN;
            m_timer.stop();

            if (m_difficulty.difficulty != CUSTOM)
            {
                m_recordTime = m_timer.query();
                if (RecordManager::isNewRecord(m_difficulty.difficulty, m_recordTime))
                    m_state = GAME_EDIT_PLAYER_ID;
            }
        }
    }
}

void Minesweeper::terminate()
{
}
} // namespace minesweeper