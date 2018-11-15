#pragma once

#include <array>
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

    bool contains(Vector2 point) const;
    Intersection getFirstIntersection(Vector2 origin, Vector2 direction) const; // Useful for Fortune's algorithm
    int getIntersections(Vector2 origin, Vector2 destination, std::array<Intersection, 2>& intersections) const; // Useful for diagram intersection
};

