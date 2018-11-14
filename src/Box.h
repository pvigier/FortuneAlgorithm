#pragma once

#include <array>
#include "Vector2.h"

class Box
{
public:
    // Be careful, y-axis is oriented to the top like in math
    enum class Side : int {LEFT, BOTTOM, RIGHT, TOP};

    double left;
    double bottom;
    double right;
    double top;

    bool contains(Vector2 point) const;
    Side getFirstIntersection(Vector2 origin, Vector2 direction, Vector2& intersection) const; // Useful for Fortune's algorithm
    int getIntersections(Vector2 origin, Vector2 destination, std::array<Vector2, 2>& intersections) const; // Useful for diagram intersection
};

