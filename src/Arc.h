#pragma once

#include "VoronoiDiagram.h"

class Event;

struct Arc
{
    enum class Color{RED, BLACK};

    Arc* parent;
    Arc* left;
    Arc* right;
    const VoronoiDiagram::Site* site;
    VoronoiDiagram::HalfEdge* leftHalfEdge;
    VoronoiDiagram::HalfEdge* rightHalfEdge;
    Event* event;
    // Optimizations
    Arc* prev;
    Arc* next;
    // Only for balancing
    Color color;

    void display(std::string tabs) const;
};

