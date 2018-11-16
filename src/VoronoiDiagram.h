#pragma once

// STL
#include <vector>
#include <memory>
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
        std::size_t index;
        Vector2 point;
        //HalfEdge* incidentEdge;
    };

    struct HalfEdge
    {
        std::size_t index;
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
    const std::vector<std::unique_ptr<Vertex>>& getVertices() const;
    const std::vector<std::unique_ptr<HalfEdge>>& getHalfEdges() const;

    // Intersection with a box
    void intersect(Box box);

private:
    std::vector<Site> mSites;
    std::vector<Face> mFaces;
    std::vector<std::unique_ptr<Vertex>> mVertices;
    std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;

    // Diagram construction
    friend FortuneAlgorithm;

    Vertex* createVertex(Vector2 point);
    Vertex* createCorner(Box box, Box::Side side);
    HalfEdge* createHalfEdge(Face* face);

    // Intersection with a box
    void link(Box box, HalfEdge* start, Box::Side startSide, HalfEdge* end, Box::Side endSide);
    void removeVertex(Vertex* vertex);
    void removeHalfEdge(HalfEdge* edge);
};
