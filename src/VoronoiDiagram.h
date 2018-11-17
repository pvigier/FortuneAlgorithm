#pragma once

// STL
#include <vector>
#include <list>
// My includes
#include "Box.h"

class FortuneAlgorithm;

class VoronoiDiagram
{
public:
    struct HalfEdge;
    struct Face;

    struct Site
    {
        std::size_t index;
        Vector2 point;
        Face* face;
    };

    struct Vertex
    {
        std::list<Vertex>::iterator it;
        Vector2 point;
        //HalfEdge* incidentEdge;
    };

    struct HalfEdge
    {
        std::list<HalfEdge>::iterator it;
        Vertex* origin;
        Vertex* destination;
        HalfEdge* twin;
        Face* incidentFace;
        HalfEdge* prev;
        HalfEdge* next;
    };

    struct Face
    {
        Site* site;
        HalfEdge* outerComponent;
    };

    VoronoiDiagram(const std::vector<Vector2>& points);

    // Remove copy operations
    VoronoiDiagram(const VoronoiDiagram&) = delete;
    VoronoiDiagram& operator=(const VoronoiDiagram&) = delete;

    // Move operations
    VoronoiDiagram(VoronoiDiagram&&) = default;
    VoronoiDiagram& operator=(VoronoiDiagram&&) = default;

    // Accessors
    Site* getSite(std::size_t i);
    std::size_t getNbSites() const;
    Face* getFace(std::size_t i);
    const std::list<Vertex>& getVertices() const;
    const std::list<HalfEdge>& getHalfEdges() const;

    // Intersection with a box
    void intersect(Box box);

private:
    std::vector<Site> mSites;
    std::vector<Face> mFaces;
    std::list<Vertex> mVertices;
    std::list<HalfEdge> mHalfEdges;

    // Diagram construction
    friend FortuneAlgorithm;

    Vertex* createVertex(Vector2 point);
    Vertex* createCorner(Box box, Box::Side side);
    HalfEdge* createHalfEdge(Face* face);

    // Intersection with a box
    void link(Box box, HalfEdge* start, Box::Side startSide, HalfEdge* end, Box::Side endSide);
    void removeVertex(Vertex* vertex);
    void removeHalfEdge(HalfEdge* halfEdge);
};
