#include "Map.h"
#include <cstring> 

Map::Map()
{
    // Initialize memory to 0 safely
    std::memset(m_heights, 0, sizeof(m_heights));
}

void Map::buildLevel()
{
    for (int x = 0; x < MAP_SIZE; x++)
    {
        for (int y = 0; y < MAP_SIZE; y++)
        {
            // FIX: Use m_heights, not m_grid
            // 0 = Flat ground. This ensures no invisible walls block you.
            m_heights[x][y] = 0;
        }
    }
}

int Map::getHeight(int x, int y) const
{
    // Bounds check
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) return 0;

    // FIX: Use m_heights
    return m_heights[x][y];
}