#pragma once

#include "VoronoiDiagram.h"

class Event;

struct Node
{
    enum class Color{RED, BLACK};

    Node* parent;
    Node* left;
    Node* right;
    const VoronoiDiagram::Site* site;
    VoronoiDiagram::HalfEdge* leftHalfEdge;
    VoronoiDiagram::HalfEdge* rightHalfEdge;
    Event* event;
    // Optimizations
    Node* prev;
    Node* next;
    // Only for balancing
    Color color;

    void display(std::string tabs) const;
};

