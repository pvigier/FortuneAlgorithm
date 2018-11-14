#include "Box.h"

bool Box::contains(Vector2 point) const
{
    return point.x >= left && point.x <= right &&
        point.y >= bottom && point.y <= top;
}

Box::Side Box::getFirstIntersection(Vector2 origin, Vector2 direction, Vector2& intersection) const
{
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

int Box::getIntersections(Vector2 origin, Vector2 destination, Vector2& intersection1, Vector2& intersection2) const
{
    return 0;
}
