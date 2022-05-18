#ifndef MINESWEEPER_H_
#define MINESWEEPER_H_

#include <minesweeper/MineMap.h>
#include <minesweeper/MineMapRenderer.h>

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

    enum
    {
        GAME_INIT,
        GAME_STARTED,
        GAME_PLAYING,
        GAME_FINISHED
    } m_state;

    void showMenuBar();
    Operation getOperation();

public:
    void init(int argc, char *argv[]);
    void update();
    void renderGui();
    void terminate();
};
} // namespace minesweeper
#endif