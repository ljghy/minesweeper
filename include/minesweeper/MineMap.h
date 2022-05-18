#ifndef MINE_MAP_H_
#define MINE_MAP_H_

#include <cstdint>
#include <vector>

namespace minesweeper
{

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
    UNCOVERED_BOOM = 13
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

class MineMap
{
private:
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_mineCount;

    uint32_t m_seed;

    std::vector<std::vector<BlockInfo>> m_mineMap; // [row(height)][col(width)]

    void unhighlight();
    void boom();

public:
    MineMap();
    void init(uint16_t width, uint16_t height, uint16_t mineCount, int32_t seed = -1);
    void update(uint16_t row, uint16_t col, Operation oprt);
    GameState getGameState() const;
    BlockInfo getBlockInfo(uint16_t row, uint16_t col) const;
    inline uint16_t getWidth() const { return m_width; }
    inline uint16_t getHeight() const { return m_height; }
    inline uint16_t getMineCount() const { return m_mineCount; }

    ~MineMap() = default;
};
} // namespace minesweeper
#endif
