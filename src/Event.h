#pragma once

// My includes
#include "Vector2.h"
#include "VoronoiDiagram.h"

class Node;

class Event
{
public:
    enum class Type{SITE, CIRCLE};

    // Site event
    Event(const VoronoiDiagram::Site* site);
    // Circle event
    Event(float y, Vector2f point, Node* arc);

    const Type type;
    float y;
    int index;
    // Site event
    const VoronoiDiagram::Site* site;
    // Circle event
    Vector2f point;
    Node* arc;

};

bool operator<(const Event& lhs, const Event& rhs);
std::ostream& operator<<(std::ostream& os, const Event& event);

