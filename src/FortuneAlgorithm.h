#pragma once

// STL
#include <iostream>
#include <vector>
// My includes
#include "Vector2.h"
#include "PriorityQueue.h"
#include "VoronoiDiagram.h"
#include "Beachline.h"

class Arc;
class Event;

class FortuneAlgorithm
{
public:
    FortuneAlgorithm(std::vector<Vector2f> points);
    ~FortuneAlgorithm();

    void construct();

    VoronoiDiagram getDiagram();

private:
    VoronoiDiagram mDiagram;
    Beachline mBeachline;
    PriorityQueue<Event> mEvents;
    float mBeachlineY;

    // Algorithm
    void handleSiteEvent(Event* event);
    void handleCircleEvent(Event* event);

    // Arcs
    Arc* breakArc(Arc* arc, VoronoiDiagram::Site* site);
    void removeArc(Arc* arc, VoronoiDiagram::Vertex* vertex);

    // Breakpoint
    bool isMovingRight(const Arc* left, const Arc* right) const;
    float getInitialX(const Arc* left, const Arc* right, bool movingRight) const;

    // Edges
    void addEdge(Arc* left, Arc* right);
    void setOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
    void setDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
    void setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next);

    // Events
    void addEvent(Arc* left, Arc* middle, Arc* right);
    void deleteEvent(Arc* arc);
    Vector2f computeConvergencePoint(Vector2f point1, Vector2f point2, Vector2f point3, float& y) const;
};

