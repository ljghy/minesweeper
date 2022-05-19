#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <minesweeper/MineMap.h>
#include <minesweeper/MineMapRenderer.h>
#include <utils/Timer.hpp>

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
            width = 9;
            height = 9;
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

    enum
    {
        GAME_INIT,
        GAME_IDLE,
        GAME_PLAYING,
        GAME_WIN,
        GAME_LOSE,
        GAME_QUIT
    } m_state;

    void showMenuBar();
    void showStatistics();
    void showFinishWindow(const ImVec2 &center);
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