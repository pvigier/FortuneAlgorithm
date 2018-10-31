#pragma once

// STL
#include <vector>
#include <list>
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
        Vector2f point;
        Face* face;
    };

    struct Vertex
    {
        Vector2f point;
        HalfEdge* incidentEdge;
    };

    struct HalfEdge
    {
        const Vertex* origin;
        const Vertex* destination;
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

    VoronoiDiagram(std::vector<Vector2f> points);

    // Remove copy operations
    VoronoiDiagram(const VoronoiDiagram&) = delete;
    VoronoiDiagram& operator=(const VoronoiDiagram&) = delete;

    // Move operations
    VoronoiDiagram(VoronoiDiagram&&) = default;
    VoronoiDiagram& operator=(VoronoiDiagram&&) = default;

    const Site* getSite(std::size_t i) const;
    std::size_t getNbSites() const;
    const Face* getFace(std::size_t i) const;
    const std::list<Vertex>& getVertices() const;
    const std::list<HalfEdge>& getHalfEdges() const;

private:
    std::vector<Site> mSites;
    std::vector<Face> mFaces;
    std::list<Vertex> mVertices;
    std::list<HalfEdge> mHalfEdges;

    // Diagram construction
    friend FortuneAlgorithm;

    const Vertex* createVertex(Vector2f point);
    HalfEdge* createHalfEdge(Face* face);
};
