#pragma once
#include "Config.h"
#include "Prop.h"
#include <vector>
#include <memory> // Required for std::unique_ptr

class Map
{
private:
    int m_heights[MAP_SIZE][MAP_SIZE];

    // FIX: Store pointers so the Props don't move in memory and break their textures
    std::vector<std::unique_ptr<Prop>> m_props;

public:
    Map();
    void buildLevel();
    int getHeight(int x, int y) const;

    bool checkPropCollision(float x, float y) const;

    // Return the list of pointers
    const std::vector<std::unique_ptr<Prop>>& getProps() const { return m_props; }
};