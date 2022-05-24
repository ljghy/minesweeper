#ifndef MINE_MAP_H_
#define MINE_MAP_H_

#include <cstdint>
#include <vector>
#include <minesweeper/MinesweeperDefs.h>

namespace minesweeper
{

class MineMap
{
private:
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_mineCount;

    uint32_t m_seed;

    std::vector<std::vector<BlockInfo>> m_mineMap;        // [row(height)][col(width)]
    std::vector<std::pair<uint16_t, uint16_t>> m_minePos; //(r, c)

    void unhighlight();
    void boom();
    bool isValidRowCol(int16_t r, int16_t c);

    bool m_questionMarkEnabled;

public:
    MineMap();
    void init(uint16_t width, uint16_t height, uint16_t mineCount, int16_t excludeX = -1, int16_t excludeY = -1, int32_t seed = -1);
    void update(uint16_t row, uint16_t col, Operation oprt);
    GameState getGameState();
    BlockInfo getBlockInfo(uint16_t row, uint16_t col) const;
    inline uint16_t getWidth() const { return m_width; }
    inline uint16_t getHeight() const { return m_height; }
    inline uint16_t getMineCount() const { return m_mineCount; }

    bool isQuestionMarkEnabled() const { return m_questionMarkEnabled; }
    void enableQuestionMark();
    void disableQuestionMark();

    ~MineMap() = default;
};

} // namespace minesweeper
#endif
