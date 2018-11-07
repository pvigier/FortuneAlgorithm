#include "Event.h"

Event::Event(VoronoiDiagram::Site* site) : type(Type::SITE), y(site->point.y), index(-1), site(site)
{

}

Event::Event(double y, Vector2 point, Arc* arc) : type(Type::CIRCLE), y(y), index(-1), point(point), arc(arc)
{


}
bool operator<(const Event& lhs, const Event& rhs)
{
    return lhs.y < rhs.y;
}

std::ostream& operator<<(std::ostream& os, const Event& event)
{
    if(event.type == Event::Type::SITE)
        os << "S(" << event.site->index << ", " << event.y << ")";
    else
        os << "C(" << event.arc << ", " << event.y << ", " << event.point << ")";
    return os;
}

