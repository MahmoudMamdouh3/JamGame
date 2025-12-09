#include "Map.h"
#include <cstring> 

Map::Map()
{
    std::memset(m_heights, 0, sizeof(m_heights));
}

void Map::buildLevel()
{
    m_props.clear();

    // Reset ground
    for (int x = 0; x < MAP_SIZE; x++)
        for (int y = 0; y < MAP_SIZE; y++)
            m_heights[x][y] = 0;

    // FIX: Use std::make_unique to create props
    m_props.push_back(std::make_unique<Prop>(5.0f, 2.0f, "assets/ENV Sketch 2.png", 0.5f));
    m_props.push_back(std::make_unique<Prop>(8.0f, 8.0f, "assets/ENV Sketch 3.png", 0.5f));
    // Add as many as you want, they won't crash now
}

int Map::getHeight(int x, int y) const
{
    if (x < 0 || x >= MAP_SIZE || y < 0 || y >= MAP_SIZE) return 0;
    return m_heights[x][y];
}

bool Map::checkPropCollision(float x, float y) const
{
    // Loop through pointers
    for (const auto& prop : m_props)
    {
        // Use '->' because 'prop' is a pointer now
        if (prop->isColliding(x, y))
            return true;
    }
    return false;
}