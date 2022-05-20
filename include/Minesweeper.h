#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <minesweeper/MineMap.h>
#include <minesweeper/MineMapRenderer.h>
#include <minesweeper/DigitRenderer.h>
#include <utils/Timer.hpp>
#include <opengl_framework/common.h>
namespace minesweeper
{
enum Difficulty
{
    EASY = 0,
    NORMAL = 1,
    HARD = 2,
    CUSTOM = 3
};

inline const char *difficultyStr[]{
    "Easy",
    "Normal",
    "Hard",
    "Custom"};

struct DifficultySelection
{
    Difficulty difficulty;
    uint16_t width;
    uint16_t height;
    uint16_t mineCount;

    DifficultySelection(Difficulty d = EASY, uint16_t w = 0, uint16_t h = 0, uint16_t m = 0)
        : difficulty(d)
    {
        switch (d)
        {
        case EASY:
            width = 8;
            height = 8;
            mineCount = 10;
            break;
        case NORMAL:
            width = 16;
            height = 16;
            mineCount = 40;
            break;
        case HARD:
            width = 30;
            height = 16;
            mineCount = 99;
            break;
        case CUSTOM:
            width = w;
            height = h;
            mineCount = m;
            break;
        }
    }
};

class Minesweeper
{
private:
    MineMap m_mineMap;
    MineMapRenderer m_mineMapRenderer;

    int16_t m_mineMapMouseX;
    int16_t m_mineMapMouseY;

    bool uncoverWhenRelease;

    DifficultySelection m_difficulty;
    Timer m_timer;
    int16_t m_remainingMineCount;

    DigitRenderer m_timerIntRenderer;
    DigitRenderer m_timerDecRenderer;
    DigitRenderer m_mineCountRenderer;

    ImVec2 m_viewportCenter;

    enum
    {
        GAME_IDLE,
        GAME_PLAYING,
        GAME_WIN,
        GAME_LOSE,
        GAME_QUIT,

        GAME_EDIT_CUSTOM,
    } m_state;

    void initGame();

    void showTimer();
    void showRemainingMineCount();
    void showCustomEditor();
    void showMenuBar();
    void showStatistics();
    void showFinishWindow();
    Operation getOperation();

public:
    void init(int argc, char *argv[]);
    void update();
    void renderGui();
    bool shouldQuit() const { return m_state == GAME_QUIT; }
    void terminate();
};
} // namespace minesweeper
#endif