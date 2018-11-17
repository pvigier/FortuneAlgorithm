#pragma once

// STL
#include <array>
// My includes
#include "Vector2.h"

class Box
{
public:
    // Be careful, y-axis is oriented to the top like in math
    enum class Side : int {LEFT, BOTTOM, RIGHT, TOP};

    struct Intersection
    {
        Side side;
        Vector2 point;
    };

    double left;
    double bottom;
    double right;
    double top;

    bool contains(const Vector2& point) const;
    Intersection getFirstIntersection(const Vector2& origin, const Vector2& direction) const; // Useful for Fortune's algorithm
    int getIntersections(const Vector2& origin, const Vector2& destination, std::array<Intersection, 2>& intersections) const; // Useful for diagram intersection
};

