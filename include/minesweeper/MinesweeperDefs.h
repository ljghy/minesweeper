#ifndef MINESWEEPER_DEFS_H_
#define MINESWEEPER_DEFS_H_

#include <cstdint>

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

enum Operation
{
    HOVER = 0,
    UNCOVER_SINGLE = 1,     // left released
    UNCOVER_BATCH = 2,      // left + right released or wheel released
    MARK_BLOCK = 3,         // right click
    HIGHLIGHT_NEIGHBORS = 4 // left + right pressed or wheel pressed
};

constexpr uint8_t UNCOVERED_START = 4;

enum BlockState
{
    COVERED = 0,
    COVERED_FLAGGED = 1,
    COVERED_UNDETERMINED = 2,

    UNCOVERED_0 = 4,
    UNCOVERED_1 = 5,
    UNCOVERED_2 = 6,
    UNCOVERED_3 = 7,
    UNCOVERED_4 = 8,
    UNCOVERED_5 = 9,
    UNCOVERED_6 = 10,
    UNCOVERED_7 = 11,
    UNCOVERED_8 = 12,
    UNCOVERED_BOOM = 13,
    UNCOVERED_FLAGGED = 14,
};

struct BlockInfo
{
    bool isMine = false;
    BlockState state = COVERED;
    bool isHighlighted = false;
    uint8_t neighborMineCount = 0;

    bool isCovered() const { return state < UNCOVERED_START; }
    bool isUncovered() const { return state >= UNCOVERED_START; }
    bool isFlagged() const { return state == COVERED_FLAGGED; }
};

struct GameState
{
    enum
    {
        WIN,
        LOSE,
        CONTINUE
    } state;
    int16_t remainingMineCount;
};

inline const char recordFileName[] = "../res/_records";

inline constexpr uint8_t recordCount = 10;
inline constexpr uint8_t playerIDMaxLen = 32;
} // namespace minesweeper

#endif
