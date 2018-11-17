#pragma once

// My includes
#include "VoronoiDiagram.h"

class Event;

struct Arc
{
    enum class Color{RED, BLACK};

    // Hierarchy
    Arc* parent;
    Arc* left;
    Arc* right;
    // Diagram
    VoronoiDiagram::Site* site;
    VoronoiDiagram::HalfEdge* leftHalfEdge;
    VoronoiDiagram::HalfEdge* rightHalfEdge;
    Event* event;
    // Optimizations
    Arc* prev;
    Arc* next;
    // Only for balancing
    Color color;
};

