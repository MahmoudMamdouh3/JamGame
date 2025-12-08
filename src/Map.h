#pragma once
#include "Config.h"

class Map
{
private:
    int m_heights[MAP_SIZE][MAP_SIZE];

public:
    Map();

    void buildLevel();

    // Returns the height of the block at grid coordinates (x, y)
    int getHeight(int x, int y) const;
};