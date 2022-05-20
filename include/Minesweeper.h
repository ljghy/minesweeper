#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <minesweeper/MineMap.h>
#include <minesweeper/MineMapRenderer.h>
#include <minesweeper/DigitRenderer.h>
#include <minesweeper/MinesweeperDefs.h>
#include <utils/Timer.hpp>
#include <opengl_framework/common.h>
namespace minesweeper
{

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

    float m_recordTime;
    uint8_t m_recordRank;

    enum
    {
        GAME_IDLE,
        GAME_PLAYING,
        GAME_WIN,
        GAME_LOSE,
        GAME_QUIT,

        GAME_EDIT_CUSTOM,
        GAME_EDIT_PLAYER_ID,
    } m_state;

    void initGame();

    void showTimer();
    void showRemainingMineCount();
    void showCustomEditor();
    void showPlayerIDEditor();
    void showMenuBar();
    void showStatistics();
    void showRecords();
    void showFinishWindow();
    Operation getOperation();

public:
    Minesweeper();
    void init(int argc, char *argv[]);
    void update();
    void renderGui();
    bool shouldQuit() const { return m_state == GAME_QUIT; }
    void terminate();
};
} // namespace minesweeper
#endif