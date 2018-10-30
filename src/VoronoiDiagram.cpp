#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(std::vector<Vector2f> points)
{
    mSites.reserve(points.size());
    mFaces.reserve(points.size());
    for(std::size_t i = 0; i < points.size(); ++i)
    {
        mSites.push_back(VoronoiDiagram::Site{i, points[i], nullptr});
        mFaces.push_back(VoronoiDiagram::Face{&mSites.back(), nullptr});
        mSites.back().face = &mFaces.back();
    }
}

const VoronoiDiagram::Vertex* VoronoiDiagram::createVertex(Vector2f point)
{
    mVertices.push_back(VoronoiDiagram::Vertex{point, nullptr});
    return &mVertices.back();
}

VoronoiDiagram::HalfEdge* VoronoiDiagram::createHalfEdge(Face* face)
{
    mHalfEdges.push_back(VoronoiDiagram::HalfEdge{nullptr, nullptr, nullptr, face, nullptr, nullptr});
    if(face->outerComponent == nullptr)
        face->outerComponent = &mHalfEdges.back();
    return &mHalfEdges.back();
}

const VoronoiDiagram::Site* VoronoiDiagram::getSite(std::size_t i) const
{
    return &mSites[i];
}

std::size_t VoronoiDiagram::getNbSites() const
{
    return mSites.size();
}

const std::list<VoronoiDiagram::Vertex>& VoronoiDiagram::getVertices() const
{
    return mVertices;
}

const std::list<VoronoiDiagram::HalfEdge>& VoronoiDiagram::getHalfEdges() const
{
    return mHalfEdges;
}

const VoronoiDiagram::Face* VoronoiDiagram::getFace(std::size_t i) const
{
    return &mFaces[i];
}