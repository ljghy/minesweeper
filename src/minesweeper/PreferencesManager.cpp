#include "PreferencesManager.h"
#include <fstream>

namespace minesweeper
{

void PreferencesManager::load()
{
    std::ifstream file(preferencesFileName, std::ios::binary);
    if (!file.is_open())
    {
        write();
        return;
    }

    uint8_t d;
    uint16_t w, h, m;
    file.read(reinterpret_cast<char *>(&d), 1);
    file.read(reinterpret_cast<char *>(&w), 2);
    file.read(reinterpret_cast<char *>(&h), 2);
    file.read(reinterpret_cast<char *>(&m), 2);
    auto &p = preferences;
    p.difficulty = DifficultySelection(static_cast<Difficulty>(d), w, h, m);

    file.read(reinterpret_cast<char *>(&d), 1);
    p.imGuiStyleColor = static_cast<ImGuiStyleColor>(d);

    file.read(reinterpret_cast<char *>(p.backgroundColor.data()), p.backgroundColor.size() * sizeof(float));
    file.read(reinterpret_cast<char *>(&p.mineMapOpacity), sizeof(float));

    file.read(reinterpret_cast<char *>(&d), 1);
    p.showBackgroundImage = static_cast<bool>(d);

    file.read(reinterpret_cast<char *>(&p.fontSize), 1);
    file.close();
}

void PreferencesManager::write()
{
    std::ofstream file(preferencesFileName, std::ios::binary | std::ios::trunc);

    auto &p = preferences;
    uint8_t d = static_cast<uint8_t>(p.difficulty.difficulty);

    file.write(reinterpret_cast<const char *>(&d), 1);
    file.write(reinterpret_cast<const char *>(&p.difficulty.width), 2);
    file.write(reinterpret_cast<const char *>(&p.difficulty.height), 2);
    file.write(reinterpret_cast<const char *>(&p.difficulty.mineCount), 2);

    d = static_cast<uint8_t>(p.imGuiStyleColor);
    file.write(reinterpret_cast<const char *>(&d), 1);

    file.write(reinterpret_cast<const char *>(p.backgroundColor.data()), p.backgroundColor.size() * sizeof(float));
    file.write(reinterpret_cast<const char *>(&p.mineMapOpacity), sizeof(float));

    d = static_cast<uint8_t>(p.showBackgroundImage);
    file.write(reinterpret_cast<const char *>(&d), 1);

    file.write(reinterpret_cast<const char *>(&p.fontSize), 1);

    file.close();
}
} // namespace minesweeper