#pragma once
#include "Config.h"

class Map
{
private:
    int m_heights[MAP_SIZE][MAP_SIZE];

public:
    Map();

    void buildLevel();
    int getHeight(int x, int y) const;
    int (*getHeights()) [MAP_SIZE]
    { return m_heights; }
};
