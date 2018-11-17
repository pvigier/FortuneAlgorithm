#include "Box.h"

bool Box::contains(Vector2 point) const
{
    return point.x >= left && point.x <= right &&
        point.y >= bottom && point.y <= top;
}

Box::Intersection Box::getFirstIntersection(Vector2 origin, Vector2 direction) const
{
    // origin must be in the box
    Intersection intersection;
    double t;
    if (direction.x > 0.0)
    {
        t = (right - origin.x) / direction.x;
        intersection.side = Side::RIGHT;
        intersection.point = origin + t * direction;
    }
    else if (direction.x < 0.0)
    {
        t = (left - origin.x) / direction.x;
        intersection.side = Side::LEFT;
        intersection.point = origin + t * direction;
    }
    if (direction.y > 0.0)
    {
        double newT = (top - origin.y) / direction.y;
        if (newT < t)
        {
            intersection.side = Side::TOP;
            intersection.point = origin + newT * direction;
        }
    }
    else if (direction.y < 0.0)
    {
        double newT = (bottom - origin.y) / direction.y;
        if (newT < t)
        {
            intersection.side = Side::BOTTOM;
            intersection.point = origin + newT * direction;
        }
    }
    return intersection;
}

int Box::getIntersections(Vector2 origin, Vector2 destination, std::array<Intersection, 2>& intersections) const
{
    // WARNING: If the intersection is a corner, both intersections are equals
    Vector2 direction = destination - origin;
    std::array<double, 2> t;
    std::size_t i = 0; // index of the current intersection
    // Left
    t[i] = (left - origin.x) / direction.x;
    if (t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i].side = Side::LEFT;
        intersections[i].point = origin + t[i] * direction;
        if (intersections[i].point.y >= bottom && intersections[i].point.y <= top)
            ++i;
    }
    // Right
    t[i] = (right - origin.x) / direction.x;
    if (t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i].side = Side::RIGHT;
        intersections[i].point = origin + t[i] * direction;
        if (intersections[i].point.y >= bottom && intersections[i].point.y <= top)
            ++i;
    }
    // Bottom
    t[i] = (bottom - origin.y) / direction.y;
    if (i < 2 && t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i].side = Side::BOTTOM;
        intersections[i].point = origin + t[i] * direction;
        if (intersections[i].point.x >= left && intersections[i].point.x <= right)
            ++i;
    }
    // Top
    t[i] = (top - origin.y) / direction.y;
    if (i < 2 && t[i] >= 0.0 && t[i] <= 1.0)  
    {
        intersections[i].side = Side::TOP;
        intersections[i].point = origin + t[i] * direction;
        if (intersections[i].point.x >= left && intersections[i].point.x <= right)
            ++i;
    }
    // Sort the intersections from the nearest to the farthest
    if (i == 2 && t[0] > t[1])
        std::swap(intersections[0], intersections[1]);
    return i;
}
