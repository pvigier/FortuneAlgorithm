#pragma once

// STL
#include <vector>
#include <memory>
// My includes
#include "Vector2.h"

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

    Site* getSite(std::size_t i);
    std::size_t getNbSites() const;
    Face* getFace(std::size_t i);
    const std::vector<std::unique_ptr<Vertex>>& getVertices() const;
    const std::vector<std::unique_ptr<HalfEdge>>& getHalfEdges() const;

private:
    std::vector<Site> mSites;
    std::vector<Face> mFaces;
    std::vector<std::unique_ptr<Vertex>> mVertices;
    std::vector<std::unique_ptr<HalfEdge>> mHalfEdges;

    // Diagram construction
    friend FortuneAlgorithm;

    Vertex* createVertex(Vector2 point);
    HalfEdge* createHalfEdge(Face* face);

    // Diagram intersection
    void removeVertex(Vertex* vertex);
    void removeHalfEdge(HalfEdge* edge);
};
