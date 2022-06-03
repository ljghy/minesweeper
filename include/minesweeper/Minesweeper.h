#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <minesweeper/MineMap.h>
#include <minesweeper/MineMapRenderer.h>
#include <minesweeper/DigitRenderer.h>
#include <minesweeper/MinesweeperDefs.h>
#include <minesweeper/PreferencesManager.h>
#include <utils/Timer.hpp>
#include <opengl_framework/common.h>
namespace minesweeper
{

class Minesweeper
{
private:
    MineMap m_mineMap;

    int16_t m_mineMapMouseX;
    int16_t m_mineMapMouseY;
    int16_t m_remainingMineCount;
    ImVec2 m_viewportCenter;
    float m_recordTime;
    uint8_t m_recordRank;

    DifficultySelection m_difficulty;

    Timer m_timer;
    MineMapRenderer m_mineMapRenderer;
    DigitRenderer m_timerIntRenderer;
    DigitRenderer m_timerDecRenderer;
    DigitRenderer m_mineCountRenderer;

    enum
    {
        GAME_IDLE,
        GAME_PLAYING,
        GAME_WIN,
        GAME_WIN_NEW_RECORD,
        GAME_LOSE,
        GAME_QUIT,

        UI_EDIT_CUSTOM,
        UI_EDIT_PLAYER_ID,
        UI_SHOW_RECORDS_WINDOW,
        UI_EDIT_PREFERENCES,
    } m_state,
        m_prevState;

    Preferences m_prevPref;

    void initGame();

    void showTimer();
    void showRemainingMineCount();
    void showCustomEditor();
    void showPlayerIDEditor();
    void showPreferencesEditor();
    void showMenuBar();
    void showStatistics(int col);
    void showRecords();
    void showRecordsWindow();
    void showFinishWindow();
    void showFinishWindowWithRecords();
    void showMineMap();

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