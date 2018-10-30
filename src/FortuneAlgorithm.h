#pragma once

// STL
#include <iostream>
#include <vector>
// My includes
#include "Vector2.h"
#include "PriorityQueue.h"
#include "VoronoiDiagram.h"
#include "Beachline.h"

class Node;
class Event;

class FortuneAlgorithm
{
public:
    FortuneAlgorithm(std::vector<Vector2f> points);

    void construct();

    VoronoiDiagram getDiagram() const;

private:
    VoronoiDiagram mDiagram;
    Beachline mBeachline;
    PriorityQueue<Event> mEvents;
    float mBeachlineY;

    // Algorithm
    void handleSiteEvent(Event* event);
    void handleCircleEvent(Event* event);

    // Arcs
    Node* breakArc(Node* arc, const VoronoiDiagram::Site* site);
    void removeArc(Node* arc, const VoronoiDiagram::Vertex* vertex);

    // Breakpoint
    bool isMovingRight(const Node* left, const Node* right) const;
    float getInitialX(const Node* left, const Node* right, bool movingRight);

    // Edges
    void addEdge(Node* left, Node* right);
    void setOrigin(Node* left, Node* right, const VoronoiDiagram::Vertex* vertex);
    void setDestination(Node* left, Node* right, const VoronoiDiagram::Vertex* vertex);
    void setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next);

    // Events
    void addEvent(Node* left, Node* middle, Node* right);
    void deleteEvent(Node* arc);
    Vector2f computeConvergencePoint(Vector2f point1, Vector2f point2, Vector2f point3, float& y);

    // tmp
    void free(Node* node);
};

