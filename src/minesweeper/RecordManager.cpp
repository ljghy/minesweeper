#include <minesweeper/RecordManager.h>
#include <fstream>
#include <algorithm>
#include <cassert>
#include <sstream>
#include <iomanip>
#include <chrono>

namespace minesweeper
{

std::array<std::array<Record, recordCount>, 3> RecordManager::records;

void RecordManager::init()
{
    for (uint8_t i = 0; i < 3; ++i)
        std::fill(records[i].begin(), records[i].end(), Record{"noname", static_cast<Difficulty>(i), 999.999f, "YYYY-mm-dd-HH:MM:SS"});
}

void RecordManager::load()
{
    std::ifstream file(recordFileName, std::ios::binary);
    if (!file.is_open())
    {
        write();
        return;
    }
    for (uint8_t i = 0; i < 3; ++i)
        for (auto &rec : records[i])
        {
            uint16_t len;
            file.read(reinterpret_cast<char *>(&len), 2);
            char *buffer = new char[len];
            file.read(buffer, len);
            rec.playerID = std::string(buffer, len);
            delete[] buffer;
            file.read(reinterpret_cast<char *>(&rec.time), sizeof(float));

            file.read(reinterpret_cast<char *>(&len), 2);
            buffer = new char[len];
            file.read(buffer, len);
            rec.timeStamp = std::string(buffer, len);
            delete[] buffer;
        }
    file.close();
}

void RecordManager::write()
{
    std::ofstream file(recordFileName, std::ios::binary | std::ios::trunc);
    for (auto &d : records)
        for (auto &rec : d)
        {
            uint16_t len = static_cast<uint16_t>(rec.playerID.size());
            file.write(reinterpret_cast<const char *>(&len), 2);
            file.write(rec.playerID.data(), rec.playerID.size());
            file.write(reinterpret_cast<const char *>(&rec.time), sizeof(float));

            len = static_cast<uint16_t>(rec.timeStamp.size());
            file.write(reinterpret_cast<const char *>(&len), 2);
            file.write(rec.timeStamp.data(), rec.timeStamp.size());
        }
    file.close();
}

std::string RecordManager::getTimeStamp()
{
    auto timePoint = std::chrono::system_clock::now();
    std::time_t time = std::chrono::system_clock::to_time_t(timePoint);
    std::tm *tm = std::localtime(&time);
    std::stringstream ss;
    ss << std::put_time(tm, "%Y-%m-%d-%H:%M:%S");
    return ss.str();
}

bool RecordManager::isNewRecord(Difficulty d, float time)
{
    return time <= records[d].back().time;
}

uint8_t RecordManager::insertRecord(const std::string &playerID, Difficulty d, float time)
{
    assert(d != CUSTOM);
    auto &recs = records[d];
    assert(time <= recs.back().time);
    uint8_t i = recordCount - 1;
    while (i > 0)
    {
        if (recs[i].time >= time && recs[i - 1].time < time)
            break;
        --i;
    }
    for (uint8_t j = recordCount - 1; j > i; --j)
        recs[j] = recs[j - 1];
    recs[i] = Record{playerID, d, time, getTimeStamp()};
    return i;
}

} // namespace minesweeper