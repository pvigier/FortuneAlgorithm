#pragma once

// My includes
#include "PriorityQueue.h"
#include "VoronoiDiagram.h"
#include "Beachline.h"

class Arc;
class Event;

class FortuneAlgorithm
{
public:
    
    FortuneAlgorithm(std::vector<Vector2> points);
    ~FortuneAlgorithm();

    void construct();
    bool bound(Box box);

    VoronoiDiagram getDiagram();

private:
    VoronoiDiagram mDiagram;
    Beachline mBeachline;
    PriorityQueue<Event> mEvents;
    double mBeachlineY;

    // Algorithm
    void handleSiteEvent(Event* event);
    void handleCircleEvent(Event* event);

    // Arcs
    Arc* breakArc(Arc* arc, VoronoiDiagram::Site* site);
    void removeArc(Arc* arc, VoronoiDiagram::Vertex* vertex);

    // Breakpoint
    bool isMovingRight(const Arc* left, const Arc* right) const;
    double getInitialX(const Arc* left, const Arc* right, bool movingRight) const;

    // Edges
    void addEdge(Arc* left, Arc* right);
    void setOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
    void setDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex);
    void setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next);

    // Events
    void addEvent(Arc* left, Arc* middle, Arc* right);
    void deleteEvent(Arc* arc);
    Vector2 computeConvergencePoint(const Vector2& point1, const Vector2& point2, const Vector2& point3, double& y) const;

    // Bounding

    struct LinkedVertex
    {
        VoronoiDiagram::HalfEdge* prevHalfEdge;
        VoronoiDiagram::Vertex* vertex;
        VoronoiDiagram::HalfEdge* nextHalfEdge;
    };
};

