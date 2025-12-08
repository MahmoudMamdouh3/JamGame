#include "Map.h"
#include <cstdlib>

Map::Map()
{
    buildLevel();
}

void Map::buildLevel()
{
    // 1. Fill ground
    for (int x = 0; x < MAP_SIZE; x++)
        for (int y = 0; y < MAP_SIZE; y++)
            m_heights[x][y] = 0; // Ground level

    // 2. Random City Generator
    // Create 40 random buildings
    for (int i = 0; i < 40; i++)
    {
        int bx = rand() % (MAP_SIZE - 4);
        int by = rand() % (MAP_SIZE - 4);
        int w = (rand() % 4) + 1;      // Width 1-4
        int h = (rand() % 4) + 1;      // Depth 1-4
        int height = (rand() % 3) + 1; // Height 1-3 blocks

        for (int x = bx; x < bx + w; x++)
        {
            for (int y = by; y < by + h; y++)
            {
                if (x < MAP_SIZE && y < MAP_SIZE)
                {
                    m_heights[x][y] = height;
                }
            }
        }
    }

    // Ensure player start position is flat (so you don't spawn inside a wall)
    int center = MAP_SIZE / 2;
    m_heights[center][center] = 0;
    m_heights[center + 1][center] = 0;
    m_heights[center][center + 1] = 0;
    m_heights[center + 1][center + 1] = 0;
}

int Map::getHeight(int x, int y) const
{
    if (x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE)
    {
        return m_heights[x][y];
    }
    return 0;
}