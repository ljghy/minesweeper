#ifndef PREFERENCES_MANAGER_H_
#define PREFERENCES_MANAGER_H_

#include <minesweeper/MinesweeperDefs.h>
#include <array>

namespace minesweeper
{

struct Preferences
{
    DifficultySelection difficulty;
    int imGuiStyleColor;

    std::array<float, 4> backgroundColor;
    float mineMapOpacity;

    bool showBackgroundImage;

    uint8_t fontSize;
};

class PreferencesManager
{
public:
    static Preferences preferences;
    static void load();
    static void write();
};

// fallback
inline Preferences PreferencesManager::preferences{EASY,
                                                   static_cast<int>(STYLE_COLOR_CLASSIC),
                                                   {0.9f, 0.9f, 0.9f, 0.25f},
                                                   0.5f,
                                                   false,
                                                   14};

} // namespace minesweeper
#endif