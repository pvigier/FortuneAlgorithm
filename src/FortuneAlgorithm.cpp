#include "FortuneAlgorithm.h"
#include "Arc.h"
#include "Event.h"

FortuneAlgorithm::FortuneAlgorithm(std::vector<Vector2f> points) : mDiagram(std::move(points))
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
    Vector2f point = event->point;
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

float FortuneAlgorithm::getInitialX(const Arc* left, const Arc* right, bool movingRight) const
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
    float y;
    Vector2f convergencePoint = computeConvergencePoint(left->site->point, middle->site->point, right->site->point, y);
    bool isBelow = ((mBeachlineY - y) / std::abs(mBeachlineY)) > 1e-6;
    bool leftBreakpointMovingRight = isMovingRight(left, middle);
    bool rightBreakpointMovingRight = isMovingRight(middle, right);
    float leftInitialX = getInitialX(left, middle, leftBreakpointMovingRight);
    float rightInitialX = getInitialX(middle, right, rightBreakpointMovingRight);
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

Vector2f FortuneAlgorithm::computeConvergencePoint(Vector2f point1, Vector2f point2, Vector2f point3, float& y) const
{
    Vector2f v1 = (point1 - point2).getOrthogonal();
    Vector2f v2 = (point2 - point3).getOrthogonal();
    Vector2f delta = 0.5f * (point3 - point1);
    float t = delta.getDet(v2) / v1.getDet(v2);
    Vector2f center = 0.5f * (point1 + point2) + t * v1;
    float r = center.getDistance(point1);
    y = center.y - r;
    return center;
}

// Bound

#include <tuple>
#include <algorithm>
#include <deque>

void FortuneAlgorithm::bound(Box box)
{
    // Retrieve all non bounded half edges from the beach line
    std::cout << mBeachline << std::endl;
    std::vector<std::array<std::deque<std::tuple<VoronoiDiagram::Vertex*, VoronoiDiagram::HalfEdge*, bool>>, 4>> vertices(mDiagram.getNbSites());
    if (!mBeachline.isEmpty())
    {
        Arc* leftArc = mBeachline.getLeftmostArc();
        Arc* rightArc = leftArc->next;
        while (!mBeachline.isNil(rightArc))
        {
            //std::cout << leftArc->site->index << ' ' << rightArc->site->index << ' ' << leftArc->rightHalfEdge << ' ' << rightArc->leftHalfEdge << std::endl;
            // Bound the edge
            Vector2f direction = (leftArc->site->point - rightArc->site->point).getOrthogonal();
            Vector2f origin = (leftArc->site->point + rightArc->site->point) * 0.5f;
            // Line-box intersection
            Vector2f intersection;
            Side side = getBoxIntersection(box, origin, direction, intersection);
            std::cout << static_cast<int>(side) << ' ' << intersection << std::endl;
            // Create a new vertex and ends the half edges
            VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(intersection);
            setDestination(leftArc, rightArc, vertex);
            // Store the vertex on the boundaries
            vertices[leftArc->site->index][static_cast<int>(side)].emplace_back(std::make_tuple(vertex, leftArc->rightHalfEdge, false));
            vertices[rightArc->site->index][static_cast<int>(side)].emplace_back(std::make_tuple(vertex, rightArc->leftHalfEdge, true));
            // Next edge
            leftArc = rightArc;
            rightArc = rightArc->next;
        }
    }
    // Sort the vertices in trigonometric order for each side
    for (std::size_t i = 0; i < mDiagram.getNbSites(); ++i)
    {
        std::sort(vertices[i][0].begin(), vertices[i][0].end(), 
            [](const auto& lhs, const auto& rhs){ return std::get<0>(lhs)->point.y > std::get<0>(rhs)->point.y; });
        std::sort(vertices[i][1].begin(), vertices[i][1].end(), 
            [](const auto& lhs, const auto& rhs){ return std::get<0>(lhs)->point.x < std::get<0>(rhs)->point.x; });
        std::sort(vertices[i][2].begin(), vertices[i][2].end(), 
            [](const auto& lhs, const auto& rhs){ return std::get<0>(lhs)->point.y < std::get<0>(rhs)->point.y; });
        std::sort(vertices[i][3].begin(), vertices[i][3].end(), 
            [](const auto& lhs, const auto& rhs){ return std::get<0>(lhs)->point.x > std::get<0>(rhs)->point.x; });
    }
    // Join the half edges
    std::array<VoronoiDiagram::Vertex*, 4> corners;
    corners.fill(nullptr);
    for (std::size_t i = 0; i < mDiagram.getNbSites(); ++i)
    {
        std::cout << "\nSite " << i << std::endl;
        for (std::size_t side = 0; side < 4; ++side)
        {
            std::cout << "Side " << side << std::endl;
            for (std::size_t j = 0; j < vertices[i][side].size(); ++j)
            {
                std::cout << "Vertex " << j << ' ' << std::get<0>(vertices[i][side][j])->point << ' ' << std::get<2>(vertices[i][side][j]) << std::endl;
                VoronoiDiagram::HalfEdge* halfEdge = std::get<1>(vertices[i][side][j]);
                bool isOrigin = std::get<2>(vertices[i][side][j]);
                if (isOrigin)
                {
                    // Find next vertex
                    VoronoiDiagram::Vertex* nextVertex;
                    if (j == vertices[i][side].size() - 1)
                    {
                        std::size_t nextSide = (side + 1) % 4;
                        if (corners[nextSide] == nullptr)
                            corners[nextSide] = createCorner(box, static_cast<Side>(nextSide));
                        nextVertex = corners[nextSide];
                    }
                    else
                        nextVertex = std::get<0>(vertices[i][side][j + 1]);
                    // Create a new half edge
                    VoronoiDiagram::HalfEdge* newHalfEdge = mDiagram.createHalfEdge(mDiagram.getFace(i));
                    newHalfEdge->origin = std::get<0>(vertices[i][side][j]);
                    newHalfEdge->destination = nextVertex;
                    newHalfEdge->prev = halfEdge;
                    halfEdge->next = newHalfEdge;
                    if (j == vertices[i][side].size() - 1)
                    {
                        vertices[i][(side + 1) % 4].push_front(std::make_tuple(nextVertex, newHalfEdge, true));
                    }
                }
                else
                {
                    // Find previous edge
                    VoronoiDiagram::HalfEdge* prevHalfEdge;
                    if (j == 0)
                    {
                        VoronoiDiagram::Vertex* prevVertex;
                        if (corners[side] == nullptr)
                            corners[side] = createCorner(box, static_cast<Side>(side));
                        prevVertex = corners[side];
                        // Create a new half edge
                        prevHalfEdge = mDiagram.createHalfEdge(mDiagram.getFace(i));
                        prevHalfEdge->origin = prevVertex;
                        prevHalfEdge->destination = std::get<0>(vertices[i][side][j]);
                        vertices[i][(side + 3) % 4].push_back(std::make_tuple(prevVertex, prevHalfEdge, false));
                    }
                    else
                        prevHalfEdge = std::get<1>(vertices[i][side][j - 1])->next;
                    prevHalfEdge->next = halfEdge;
                    halfEdge->prev = prevHalfEdge;
                }
            }
        }
    }
}

FortuneAlgorithm::Side FortuneAlgorithm::getBoxIntersection(Box box, Vector2f origin, Vector2f direction, Vector2f& intersection) const
{
    std::cout << origin << ' ' << direction << std::endl;
    Side side;
    float t;
    if (direction.x > 0)
    {
        t = (box.right - origin.x) / direction.x;
        side = Side::RIGHT;
        intersection = origin + t * direction;
    }
    else
    {
        t = (box.left - origin.x) / direction.x;
        side = Side::LEFT;
        intersection = origin + t * direction;
    }
    if (direction.y > 0)
    {
        float newT = (box.top - origin.y) / direction.y;
        if (newT < t)
        {
            side = Side::TOP;
            intersection = origin + newT * direction;
        }
    }
    else
    {
        float newT = (box.bottom - origin.y) / direction.y;
        if (newT < t)
        {
            side = Side::BOTTOM;
            intersection = origin + newT * direction;
        }
    }
    return side;
}

VoronoiDiagram::Vertex* FortuneAlgorithm::createCorner(Box box, Side side)
{
    switch (side)
    {
        case Side::LEFT:
            return mDiagram.createVertex(Vector2f(box.left, box.top));
        case Side::BOTTOM:
            return mDiagram.createVertex(Vector2f(box.left, box.bottom));
        case Side::RIGHT:
            return mDiagram.createVertex(Vector2f(box.right, box.bottom));
        case Side::TOP:
            return mDiagram.createVertex(Vector2f(box.right, box.top));
        default:
            return nullptr;
    }
}
