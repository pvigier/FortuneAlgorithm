/* FortuneAlgorithm
 * Copyright (C) 2018 Pierre Vigier
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
        Vector2 point;

    private:
        friend VoronoiDiagram;
        std::list<Vertex>::iterator it;
    };

    struct HalfEdge
    {
        Vertex* origin = nullptr;
        Vertex* destination = nullptr;
        HalfEdge* twin = nullptr;
        Face* incidentFace;
        HalfEdge* prev = nullptr;
        HalfEdge* next = nullptr;

    private:
        friend VoronoiDiagram;
        std::list<HalfEdge>::iterator it;
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
    bool intersect(Box box);

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
