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
    // Be careful, y-axis is oriented to the top like in math
    struct Box
    {
        double left;
        double bottom;
        double right;
        double top;
    };

    FortuneAlgorithm(std::vector<Vector2> points);
    ~FortuneAlgorithm();

    void construct();
    void bound(Box box);

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
    Vector2 computeConvergencePoint(Vector2 point1, Vector2 point2, Vector2 point3, double& y) const;

    // Bounding
    enum class Side : int {LEFT, BOTTOM, RIGHT, TOP};

    struct LinkedVertex
    {
        VoronoiDiagram::HalfEdge* prevHalfEdge;
        VoronoiDiagram::Vertex* vertex;
        VoronoiDiagram::HalfEdge* nextHalfEdge;
    };

    Side getBoxIntersection(Box box, Vector2 origin, Vector2 direction, Vector2& intersection) const; 
    VoronoiDiagram::Vertex* createCorner(Box box, Side side);
};

