#pragma once
#include "../Core/Config.h"
#include "../Props/Prop.h"
#include <vector>
#include <memory> // Required for unique_ptr

class Map
{
private:
    int m_heights[MAP_SIZE][MAP_SIZE];

    // FIX: Using unique_ptr to prevent memory crashes (vector resizing)
    std::vector<std::unique_ptr<Prop>> m_props;

public:
    Map();
    void buildLevel();
    int getHeight(int x, int y) const;
    bool checkPropCollision(float x, float y) const;

    const std::vector<std::unique_ptr<Prop>>& getProps() const { return m_props; }
};