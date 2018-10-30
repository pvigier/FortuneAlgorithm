#pragma once

#include "VoronoiDiagram.h"

class Event;

struct Node
{
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

    Node(const VoronoiDiagram::Site* site, Node* parent = nullptr);

    bool isLeaf() const;
    void display(std::string tabs) const;
};

