#include "Box.h"

bool Box::contains(Vector2 point) const
{
    return point.x >= left && point.x <= right &&
        point.y >= bottom && point.y <= top;
}

Box::Side Box::getFirstIntersection(Vector2 origin, Vector2 direction, Vector2& intersection) const
{
    // origin must be in the box
    Side side;
    double t;
    if (direction.x > 0.0)
    {
        t = (right - origin.x) / direction.x;
        side = Side::RIGHT;
        intersection = origin + t * direction;
    }
    else
    {
        t = (left - origin.x) / direction.x;
        side = Side::LEFT;
        intersection = origin + t * direction;
    }
    if (direction.y > 0.0)
    {
        double newT = (top - origin.y) / direction.y;
        if (newT < t)
        {
            side = Side::TOP;
            intersection = origin + newT * direction;
        }
    }
    else
    {
        double newT = (bottom - origin.y) / direction.y;
        if (newT < t)
        {
            side = Side::BOTTOM;
            intersection = origin + newT * direction;
        }
    }
    return side;
}

int Box::getIntersections(Vector2 origin, Vector2 destination, std::array<Vector2, 2>& intersections) const
{
    // WARNING: If the intersection is a corner, both intersections are equals
    Vector2 direction = destination - origin;
    std::array<double, 2> t;
    std::size_t i = 0; // index of the current intersection
    // Left
    t[i] = (left - origin.x) / direction.x;
    if (t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i] = origin + t[i] * direction;
        if (intersections[i].y >= bottom && intersections[i].y <= top)
            ++i;
    }
    // Right
    t[i] = (right - origin.x) / direction.x;
    if (t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i] = origin + t[i] * direction;
        if (intersections[i].y >= bottom && intersections[i].y <= top)
            ++i;
    }
    // Bottom
    t[i] = (bottom - origin.y) / direction.y;
    if (t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i] = origin + t[i] * direction;
        if (intersections[i].x >= left && intersections[i].x <= right)
            ++i;
    }
    // Top
    t[i] = (top - origin.y) / direction.y;
    if (t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i] = origin + t[i] * direction;
        if (intersections[i].x >= left && intersections[i].x <= right)
            ++i;
    }
    // Sort the intersections from the nearest to the farthest
    if (i == 2 && t[0] > t[1])
        std::swap(intersections[0], intersections[1]);
    return i;
}
