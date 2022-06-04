#ifndef RECORD_MANAGER_H_
#define RECORD_MANAGER_H_

#include <array>
#include <string>
#include "MinesweeperDefs.h"
namespace minesweeper
{
struct Record
{
    std::string playerID;
    Difficulty difficulty;
    float time;
    std::string timeStamp;
};

class RecordManager
{
private:
public:
    static std::array<std::array<Record, recordCount>, 3> records;
    static void init();
    static void load();
    static void write();
    static std::string getTimeStamp();
    static bool isNewRecord(Difficulty d, float time);
    static uint8_t insertRecord(const std::string &playerID, Difficulty d, float time);
};
} // namespace minesweeper

#endif