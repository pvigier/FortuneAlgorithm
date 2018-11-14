#include "VoronoiDiagram.h"

VoronoiDiagram::VoronoiDiagram(const std::vector<Vector2>& points)
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

VoronoiDiagram::Site* VoronoiDiagram::getSite(std::size_t i)
{
    return &mSites[i];
}

std::size_t VoronoiDiagram::getNbSites() const
{
    return mSites.size();
}

VoronoiDiagram::Face* VoronoiDiagram::getFace(std::size_t i)
{
    return &mFaces[i];
}

const std::vector<std::unique_ptr<VoronoiDiagram::Vertex>>& VoronoiDiagram::getVertices() const
{
    return mVertices;
}

const std::vector<std::unique_ptr<VoronoiDiagram::HalfEdge>>& VoronoiDiagram::getHalfEdges() const
{
    return mHalfEdges;
}

VoronoiDiagram::Vertex* VoronoiDiagram::createVertex(Vector2 point)
{
    mVertices.push_back(std::make_unique<Vertex>(Vertex{mVertices.size(), point/*, nullptr*/}));
    return mVertices.back().get();
}

VoronoiDiagram::HalfEdge* VoronoiDiagram::createHalfEdge(Face* face)
{
    mHalfEdges.push_back(std::make_unique<HalfEdge>(HalfEdge{mHalfEdges.size(), nullptr, nullptr, nullptr, face, nullptr, nullptr}));
    if(face->outerComponent == nullptr)
        face->outerComponent = mHalfEdges.back().get();
    return mHalfEdges.back().get();
}

void VoronoiDiagram::removeVertex(Vertex* vertex)
{
    std::swap(mVertices[vertex->index], mVertices.back());
    mVertices[vertex->index]->index = vertex->index;
    mVertices.pop_back();
}

void VoronoiDiagram::removeHalfEdge(HalfEdge* edge)
{
    std::swap(mHalfEdges[edge->index], mHalfEdges.back());
    mHalfEdges[edge->index]->index = edge->index;
    mHalfEdges.pop_back();
}

