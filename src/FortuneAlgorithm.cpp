#include "FortuneAlgorithm.h"
#include "Arc.h"
#include "Event.h"

FortuneAlgorithm::FortuneAlgorithm(std::vector<Vector2> points) : mDiagram(std::move(points))
{

}

FortuneAlgorithm::~FortuneAlgorithm() = default;

void FortuneAlgorithm::construct()
{
    // Initialize event queue
    for (std::size_t i = 0; i < mDiagram.getNbSites(); ++i)
        mEvents.push(std::make_unique<Event>(mDiagram.getSite(i)));

    // Process events
    while (!mEvents.isEmpty())
    {
        std::unique_ptr<Event> event = mEvents.pop();
        mBeachlineY = event->y;
        if(event->type == Event::Type::SITE)
            handleSiteEvent(event.get());
        else
            handleCircleEvent(event.get());
    }
}

VoronoiDiagram FortuneAlgorithm::getDiagram()
{
    return std::move(mDiagram);
}

void FortuneAlgorithm::handleSiteEvent(Event* event)
{
    VoronoiDiagram::Site* site = event->site;
    // 1. Check if the bachline is empty
    if (mBeachline.isEmpty())
    {
        mBeachline.setRoot(mBeachline.createArc(site));
        return;
    }
    // 2. Look for the arc above the site
    Arc* arcToBreak = mBeachline.locateArcAbove(site->point, mBeachlineY);
    deleteEvent(arcToBreak);
    // 3. Replace this arc by the new arcs
    Arc* middleArc = breakArc(arcToBreak, site);
    Arc* leftArc = middleArc->prev; 
    Arc* rightArc = middleArc->next;
    // 4. Add an edge in the diagram
    addEdge(leftArc, middleArc);
    middleArc->rightHalfEdge = middleArc->leftHalfEdge;
    rightArc->leftHalfEdge = leftArc->rightHalfEdge;
    // 5. Check circle events
    // Left triplet
    if (!mBeachline.isNil(leftArc->prev))
        addEvent(leftArc->prev, leftArc, middleArc);
    // Right triplet
    if (!mBeachline.isNil(rightArc->next))
        addEvent(middleArc, rightArc, rightArc->next);
}

void FortuneAlgorithm::handleCircleEvent(Event* event)
{
    Vector2 point = event->point;
    Arc* arc = event->arc;
    // 1. Add vertex
    VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(point);
    // 2. Delete all the events with this arc
    Arc* leftArc = arc->prev;
    Arc* rightArc = arc->next;
    deleteEvent(leftArc);
    deleteEvent(rightArc);
    // 3. Update the beachline and the diagram
    removeArc(arc, vertex);
    // 4. Add new circle events
    // Left triplet
    if (!mBeachline.isNil(leftArc->prev))
        addEvent(leftArc->prev, leftArc, rightArc);
    // Right triplet
    if (!mBeachline.isNil(rightArc->next))
        addEvent(leftArc, rightArc, rightArc->next);
}

Arc* FortuneAlgorithm::breakArc(Arc* arc, VoronoiDiagram::Site* site)
{
    // Create the new subtree
    Arc* middleArc = mBeachline.createArc(site);
    Arc* leftArc = mBeachline.createArc(arc->site);
    leftArc->leftHalfEdge = arc->leftHalfEdge;
    Arc* rightArc = mBeachline.createArc(arc->site);
    rightArc->rightHalfEdge = arc->rightHalfEdge;
    // Insert the subtree in the beachline
    mBeachline.replace(arc, middleArc);
    mBeachline.insertBefore(middleArc, leftArc);
    mBeachline.insertAfter(middleArc, rightArc);
    // Delete old arc
    delete arc;
    // Return the middle arc
    return middleArc;
}

void FortuneAlgorithm::removeArc(Arc* arc, VoronoiDiagram::Vertex* vertex)
{
    // End edges
    setDestination(arc->prev, arc, vertex);
    setDestination(arc, arc->next, vertex);
    // Join the edges of the middle arc
    arc->leftHalfEdge->next = arc->rightHalfEdge;
    arc->rightHalfEdge->prev = arc->leftHalfEdge;
    // Update beachline
    mBeachline.remove(arc);
    // Create a new edge
    VoronoiDiagram::HalfEdge* prevHalfEdge = arc->prev->rightHalfEdge;
    VoronoiDiagram::HalfEdge* nextHalfEdge = arc->next->leftHalfEdge;
    addEdge(arc->prev, arc->next);
    setOrigin(arc->prev, arc->next, vertex);
    setPrevHalfEdge(arc->prev->rightHalfEdge, prevHalfEdge);
    setPrevHalfEdge(nextHalfEdge, arc->next->leftHalfEdge);
    // Delete node
    delete arc;
}

bool FortuneAlgorithm::isMovingRight(const Arc* left, const Arc* right) const
{
    return left->site->point.y < right->site->point.y;
}

double FortuneAlgorithm::getInitialX(const Arc* left, const Arc* right, bool movingRight) const
{
    return movingRight ? left->site->point.x : right->site->point.x;
}

void FortuneAlgorithm::addEdge(Arc* left, Arc* right)
{
    // Create two new half edges
    left->rightHalfEdge = mDiagram.createHalfEdge(left->site->face);
    right->leftHalfEdge = mDiagram.createHalfEdge(right->site->face);
    // Set the two half edges twins
    left->rightHalfEdge->twin = right->leftHalfEdge;
    right->leftHalfEdge->twin = left->rightHalfEdge;
}

void FortuneAlgorithm::setOrigin(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->destination = vertex;
    right->leftHalfEdge->origin = vertex;
}

void FortuneAlgorithm::setDestination(Arc* left, Arc* right, VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->origin = vertex;
    right->leftHalfEdge->destination = vertex;
}

void FortuneAlgorithm::setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next)
{
    prev->next = next;
    next->prev = prev;
}

void FortuneAlgorithm::addEvent(Arc* left, Arc* middle, Arc* right)
{
    double y;
    Vector2 convergencePoint = computeConvergencePoint(left->site->point, middle->site->point, right->site->point, y);
    bool isBelow = y <= mBeachlineY;
    bool leftBreakpointMovingRight = isMovingRight(left, middle);
    bool rightBreakpointMovingRight = isMovingRight(middle, right);
    double leftInitialX = getInitialX(left, middle, leftBreakpointMovingRight);
    double rightInitialX = getInitialX(middle, right, rightBreakpointMovingRight);
    bool isValid =
        ((leftBreakpointMovingRight && leftInitialX < convergencePoint.x) ||
        (!leftBreakpointMovingRight && leftInitialX > convergencePoint.x)) &&
        ((rightBreakpointMovingRight && rightInitialX < convergencePoint.x) ||
        (!rightBreakpointMovingRight && rightInitialX > convergencePoint.x));
    if (isValid && isBelow)
    {
        std::unique_ptr<Event> event = std::make_unique<Event>(y, convergencePoint, middle);
        middle->event = event.get();
        mEvents.push(std::move(event));
    }
}

void FortuneAlgorithm::deleteEvent(Arc* arc)
{
    if (arc->event != nullptr)
    {
        mEvents.remove(arc->event->index);
        arc->event = nullptr;
    }
}

Vector2 FortuneAlgorithm::computeConvergencePoint(Vector2 point1, Vector2 point2, Vector2 point3, double& y) const
{
    Vector2 v1 = (point1 - point2).getOrthogonal();
    Vector2 v2 = (point2 - point3).getOrthogonal();
    Vector2 delta = 0.5 * (point3 - point1);
    double t = delta.getDet(v2) / v1.getDet(v2);
    Vector2 center = 0.5 * (point1 + point2) + t * v1;
    double r = center.getDistance(point1);
    y = center.y - r;
    return center;
}

// Bound

#include <algorithm>
#include <deque>
#include <list>
#include <unordered_map>

bool FortuneAlgorithm::bound(Box box)
{
    // Make sure the bounding box contains all the vertices
    for (const auto& vertex : mDiagram.getVertices())
    {
        box.left = std::min(vertex->point.x, box.left);
        box.bottom = std::min(vertex->point.y, box.bottom);
        box.right = std::max(vertex->point.x, box.right);
        box.top = std::max(vertex->point.y, box.top);
    }
    // Retrieve all non bounded half edges from the beach line
    std::list<LinkedVertex> linkedVertices;
    std::unordered_map<std::size_t, std::array<std::deque<LinkedVertex*>, 4>> vertices(mDiagram.getNbSites());
    if (!mBeachline.isEmpty())
    {
        Arc* leftArc = mBeachline.getLeftmostArc();
        Arc* rightArc = leftArc->next;
        while (!mBeachline.isNil(rightArc))
        {
            // Bound the edge
            Vector2 direction = (leftArc->site->point - rightArc->site->point).getOrthogonal();
            Vector2 origin = (leftArc->site->point + rightArc->site->point) * 0.5f;
            // Line-box intersection
            Box::Intersection intersection = box.getFirstIntersection(origin, direction);
            // Create a new vertex and ends the half edges
            VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(intersection.point);
            setDestination(leftArc, rightArc, vertex);
            // Store the vertex on the boundaries
            linkedVertices.emplace_back(LinkedVertex{nullptr, vertex, leftArc->rightHalfEdge});
            vertices[leftArc->site->index][static_cast<int>(intersection.side)].emplace_back(&linkedVertices.back());
            linkedVertices.emplace_back(LinkedVertex{rightArc->leftHalfEdge, vertex, nullptr});
            vertices[rightArc->site->index][static_cast<int>(intersection.side)].emplace_back(&linkedVertices.back());
            // Next edge
            leftArc = rightArc;
            rightArc = rightArc->next;
        }
    }
    // Sort the vertices in trigonometric order for each side of each cell
    for (auto& kv : vertices)
    {
        auto& cellVertices = kv.second;
        std::sort(cellVertices[0].begin(), cellVertices[0].end(), 
            [](const auto& lhs, const auto& rhs){ return lhs->vertex->point.y > rhs->vertex->point.y; });
        std::sort(cellVertices[1].begin(), cellVertices[1].end(), 
            [](const auto& lhs, const auto& rhs){ return lhs->vertex->point.x < rhs->vertex->point.x; });
        std::sort(cellVertices[2].begin(), cellVertices[2].end(), 
            [](const auto& lhs, const auto& rhs){ return lhs->vertex->point.y < rhs->vertex->point.y; });
        std::sort(cellVertices[3].begin(), cellVertices[3].end(), 
            [](const auto& lhs, const auto& rhs){ return lhs->vertex->point.x > rhs->vertex->point.x; });
    }
    // Add corners
    std::array<VoronoiDiagram::Vertex*, 4> corners;
    corners.fill(nullptr);
    for (auto& kv : vertices)
    {
        auto& cellVertices = kv.second;
        for (std::size_t side = 0; side < 4; ++side)
        {
            if (cellVertices[side].empty())
                continue;
            // First point is a destination
            if (cellVertices[side].front()->prevHalfEdge == nullptr && corners[side] == nullptr)
            {
                std::size_t prevSide = (side + 3) % 4;
                corners[side] = mDiagram.createCorner(box, static_cast<Box::Side>(side));
                linkedVertices.emplace_back(LinkedVertex{nullptr, corners[side], nullptr});
                cellVertices[prevSide].push_back(&linkedVertices.back());
                cellVertices[side].push_front(&linkedVertices.back());
            }
            // Last point is an origin
            std::size_t nextSide = (side + 1) % 4;
            if (cellVertices[side].back()->nextHalfEdge == nullptr && corners[nextSide] == nullptr)
            {
                corners[nextSide] = mDiagram.createCorner(box, static_cast<Box::Side>(nextSide));
                linkedVertices.emplace_back(LinkedVertex{nullptr, corners[nextSide], nullptr});
                cellVertices[side].push_back(&linkedVertices.back());
                cellVertices[nextSide].push_front(&linkedVertices.back());
            }
        }
    }
    // Join the half edges
    bool error = false;
    for (auto& kv : vertices)
    {
        std::size_t i = kv.first;
        auto& cellVertices = kv.second;
        for (std::size_t side = 0; side < 4; ++side)
        {
            if (cellVertices[side].size() == 2)
            {
                // Link vertices 
                VoronoiDiagram::HalfEdge* halfEdge = mDiagram.createHalfEdge(mDiagram.getFace(i));
                halfEdge->origin = cellVertices[side][0]->vertex;
                halfEdge->destination = cellVertices[side][1]->vertex;
                cellVertices[side][0]->nextHalfEdge = halfEdge;
                halfEdge->prev = cellVertices[side][0]->prevHalfEdge;
                if (cellVertices[side][0]->prevHalfEdge != nullptr)
                    cellVertices[side][0]->prevHalfEdge->next = halfEdge;
                cellVertices[side][1]->prevHalfEdge = halfEdge;
                halfEdge->next = cellVertices[side][1]->nextHalfEdge;
                if (cellVertices[side][1]->nextHalfEdge != nullptr)
                    cellVertices[side][1]->nextHalfEdge->prev = halfEdge;
            }
            else if (cellVertices[side].size() > 0)
                error = true;
        }
    }
    return !error;
}

