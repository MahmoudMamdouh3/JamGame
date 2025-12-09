#pragma once
#include "Config.h" // Ensure this contains MAP_SIZE

class Map
{
private:
    // This is the actual variable name
    int m_heights[MAP_SIZE][MAP_SIZE];

public:
    Map();

    void buildLevel();

    int getHeight(int x, int y) const;
};