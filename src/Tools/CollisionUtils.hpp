#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

namespace CollisionUtils {

    inline float distSq(sf::Vector2f a, sf::Vector2f b) {
        return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
    }

    // Moore-Neighbor Tracing: Finds the exact outline of an image
    inline std::vector<sf::Vector2f> getAutoContour(const sf::Image& image, unsigned int alphaThreshold = 50) {
        std::vector<sf::Vector2f> points;
        sf::Vector2u size = image.getSize();

        // 1. Find start pixel
        sf::Vector2i start(-1, -1);
        bool found = false;
        for (unsigned int y = 0; y < size.y && !found; ++y) {
            for (unsigned int x = 0; x < size.x; ++x) {
                if (image.getPixel({ x, y }).a > alphaThreshold) {
                    start = { static_cast<int>(x), static_cast<int>(y) };
                    found = true;
                    break;
                }
            }
        }
        if (!found) return points;

        // 2. Trace Outline
        sf::Vector2i offsets[] = {
            {0, -1}, {1, -1}, {1, 0}, {1, 1},
            {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
        };

        sf::Vector2i current = start;
        sf::Vector2i backtrack = { start.x, start.y - 1 };
        points.push_back(sf::Vector2f(current));
        int loopLimit = size.x * size.y * 2;

        do {
            int backtrackIndex = 0;
            for (int i = 0; i < 8; ++i) {
                if (current + offsets[i] == backtrack) {
                    backtrackIndex = i;
                    break;
                }
            }
            bool foundNext = false;
            for (int i = 0; i < 8; ++i) {
                int idx = (backtrackIndex + 1 + i) % 8;
                sf::Vector2i check = current + offsets[idx];
                if (check.x >= 0 && check.x < (int)size.x && check.y >= 0 && check.y < (int)size.y) {
                    if (image.getPixel({ (unsigned int)check.x, (unsigned int)check.y }).a > alphaThreshold) {
                        current = check;
                        backtrack = current - offsets[idx];
                        points.push_back(sf::Vector2f(current));
                        foundNext = true;
                        break;
                    }
                }
            }
            if (!foundNext) break;
        } while (current != start && --loopLimit > 0);

        // 3. Simplify (Reduce point count for performance)
        if (points.size() > 5) {
            std::vector<sf::Vector2f> simplified;
            simplified.push_back(points[0]);
            for (size_t i = 1; i < points.size(); ++i) {
                if (distSq(points[i], simplified.back()) > 25.0f) {
                    simplified.push_back(points[i]);
                }
            }
            return simplified;
        }
        return points;
    }

    // Point in Polygon Test
    inline bool isPointInsideConcave(const std::vector<sf::Vector2f>& polygon, sf::Vector2f p) {
        bool inside = false;
        size_t j = polygon.size() - 1;
        for (size_t i = 0; i < polygon.size(); i++) {
            if ((polygon[i].y > p.y) != (polygon[j].y > p.y) &&
                (p.x < (polygon[j].x - polygon[i].x) * (p.y - polygon[i].y) / (polygon[j].y - polygon[i].y) + polygon[i].x)) {
                inside = !inside;
            }
            j = i;
        }
        return inside;
    }
}