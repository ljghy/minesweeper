#include <minesweeper/MineMap.h>

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <queue>

namespace minesweeper
{

static int8_t neighborX[]{-1, 0, 1, -1, 1, -1, 0, 1};
static int8_t neighborY[]{-1, -1, -1, 0, 0, 1, 1, 1};

MineMap::MineMap()
    : m_width(0), m_height(0), m_mineCount(0)
{
}

void MineMap::init(uint16_t width, uint16_t height, uint16_t mineCount, int16_t excludeX, int16_t excludeY, int32_t seed)
{
    m_width = width;
    m_height = height;
    m_mineCount = mineCount;
    assert(m_width * m_height > m_mineCount);
    m_mineMap.resize(m_height);
    for (auto &row : m_mineMap)
        row.resize(m_width);

    for (auto &row : m_mineMap)
        std::fill(row.begin(), row.end(), BlockInfo{false, COVERED, false, 0});

    if (seed == -1)
        m_seed = static_cast<uint32_t>(time(NULL));
    else
        m_seed = static_cast<uint32_t>(seed);
    srand(m_seed);

    if (excludeX >= 0 && excludeX < width && excludeY >= 0 && excludeY < height)
        m_mineMap[excludeY][excludeX].isMine = true;
    mineCount = 0;
    m_minePos.clear();
    m_minePos.reserve(m_mineCount);
    while (mineCount < m_mineCount)
    {
        uint16_t r = rand() % m_height, c = rand() % m_width;
        if (!m_mineMap[r][c].isMine)
        {
            m_mineMap[r][c].isMine = true;
            m_minePos.push_back(std::make_pair(r, c));
            for (uint8_t i = 0; i < 8; ++i)
            {
                int16_t x = r + neighborX[i], y = c + neighborY[i];
                if (x >= 0 && x < m_height && y >= 0 && y < m_width)
                    ++m_mineMap[x][y].neighborMineCount;
            }
            ++mineCount;
        }
    }
    if (excludeX >= 0 && excludeX < width && excludeY >= 0 && excludeY < height)
        m_mineMap[excludeY][excludeX].isMine = false;
}

void MineMap::unhighlight()
{
    for (auto &row : m_mineMap)
        for (auto &b : row)
            b.isHighlighted = false;
}

void MineMap::boom()
{
    for (auto &row : m_mineMap)
        for (auto &b : row)
            if (b.isMine)
                b.state = UNCOVERED_BOOM;
}

void MineMap::update(uint16_t row, uint16_t col, Operation oprt)
{
    unhighlight();
    if (row >= m_height || col >= m_width)
        return;

    auto &block = m_mineMap[row][col];
    block.isHighlighted = true;

    if (oprt == HOVER)
    {
        return;
    }
    else if (oprt == UNCOVER_SINGLE)
    {
        if (block.state != COVERED)
            return;
        if (!block.isMine)
        {
            block.state = BlockState(UNCOVERED_START + block.neighborMineCount);
            if (block.neighborMineCount > 0)
                return;
            std::queue<std::pair<uint16_t, uint16_t>> q;
            q.push({row, col});
            while (!q.empty())
            {
                auto p = q.front();
                q.pop();
                auto &b = m_mineMap[p.first][p.second];

                for (uint8_t i = 0; i < 8; ++i)
                {
                    int16_t x = p.first + neighborX[i], y = p.second + neighborY[i];
                    if (x >= 0 && x < m_height && y >= 0 && y < m_width && !m_mineMap[x][y].isMine && m_mineMap[x][y].isCovered())
                    {
                        auto &b = m_mineMap[x][y];
                        b.state = BlockState(UNCOVERED_START + b.neighborMineCount);
                        if (b.neighborMineCount == 0)
                            q.push({x, y});
                    }
                }
            }
        }
        else
            boom();
    }
    else if (oprt == UNCOVER_BATCH)
    {
        if (block.isCovered() || block.state == UNCOVERED_0 || block.state == UNCOVERED_BOOM)
            return;
        uint8_t flags = 0, correctFlags = 0;
        for (uint8_t i = 0; i < 8; ++i)
        {
            int16_t x = row + neighborX[i], y = col + neighborY[i];
            if (x >= 0 && x < m_height && y >= 0 && y < m_width)
            {
                auto &b = m_mineMap[x][y];
                if (b.isFlagged())
                {
                    ++flags;
                    if (b.isMine)
                        ++correctFlags;
                }
            }
        }

        if (flags == block.neighborMineCount)
        {
            if (flags == correctFlags)
            {
                std::queue<std::pair<uint16_t, uint16_t>> q;
                for (uint8_t i = 0; i < 8; ++i)
                {
                    int16_t x = row + neighborX[i], y = col + neighborY[i];
                    if (x >= 0 && x < m_height && y >= 0 && y < m_width)
                    {
                        auto &b = m_mineMap[x][y];
                        if (b.state == COVERED)
                        {
                            b.state = BlockState(UNCOVERED_START + b.neighborMineCount);
                            if (b.neighborMineCount == 0)
                                q.push({x, y});
                        }
                    }
                }

                while (!q.empty())
                {
                    auto p = q.front();
                    q.pop();
                    auto &b = m_mineMap[p.first][p.second];

                    for (uint8_t i = 0; i < 8; ++i)
                    {
                        int16_t x = p.first + neighborX[i], y = p.second + neighborY[i];
                        if (x >= 0 && x < m_height && y >= 0 && y < m_width && !m_mineMap[x][y].isMine && m_mineMap[x][y].isCovered())
                        {
                            auto &b = m_mineMap[x][y];
                            b.state = BlockState(UNCOVERED_START + b.neighborMineCount);
                            if (b.neighborMineCount == 0)
                                q.push({x, y});
                        }
                    }
                }
            }
            else
                boom();
        }
    }
    else if (oprt == MARK_BLOCK)
    {
        if (block.isCovered())
        {
            if (block.state == COVERED)
                block.state = COVERED_FLAGGED;
            else if (block.state == COVERED_FLAGGED)
                block.state = COVERED_UNDETERMINED;
            else if (block.state == COVERED_UNDETERMINED)
                block.state = COVERED;
        }
    }
    else if (oprt == HIGHLIGHT_NEIGHBORS)
    {
        for (uint16_t i = std::max(0, row - 1); i < std::min(m_height, uint16_t(row + 2)); ++i)
            for (uint16_t j = std::max(0, col - 1); j < std::min(m_width, uint16_t(col + 2)); ++j)
                m_mineMap[i][j].isHighlighted = true;
    }
}

BlockInfo MineMap::getBlockInfo(uint16_t row, uint16_t col) const
{
    assert(row < m_height && col < m_width);
    return m_mineMap[row][col];
}

GameState MineMap::getGameState()
{
    GameState ret{GameState::CONTINUE, static_cast<int16_t>(m_mineCount)};
    uint16_t uncovered = 0;
    for (auto &row : m_mineMap)
        for (auto &b : row)
        {
            if (b.isFlagged())
                --ret.remainingMineCount;
            if (b.isMine && b.state == UNCOVERED_BOOM)
                ret.state = GameState::LOSE;
            if (b.state >= UNCOVERED_START && b.state < UNCOVERED_START + 9)
                ++uncovered;
        }
    if (uncovered + m_mineCount == m_height * m_width)
    {
        ret.state = GameState::WIN;
        for (const auto &p : m_minePos)
        {
            m_mineMap[p.first][p.second].state = UNCOVERED_FLAGGED;
        }
    }
    return ret;
}

} // namespace minesweeper