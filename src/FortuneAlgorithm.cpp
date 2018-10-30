#include "FortuneAlgorithm.h"
#include "Node.h"
#include "Event.h"

FortuneAlgorithm::FortuneAlgorithm(std::vector<Vector2f> points) : mDiagram(std::move(points))
{

}

void FortuneAlgorithm::construct()
{
    // Initialize event queue
    for(std::size_t i = 0; i < mDiagram.getNbSites(); ++i)
        mEvents.push(new Event(mDiagram.getSite(i)));

    // Process events
    while(!mEvents.isEmpty())
    {
        Event* event = mEvents.top();
        mEvents.pop();
        mBeachlineY = event->y;
        if(event->type == Event::Type::SITE)
            handleSiteEvent(event);
        else
            handleCircleEvent(event);
        delete event;
    }

    // Shoud empty the beachline!
    free(mBeachline.getRoot());
}

VoronoiDiagram FortuneAlgorithm::getDiagram() const
{
    return mDiagram;
}

void FortuneAlgorithm::handleSiteEvent(Event* event)
{
    const VoronoiDiagram::Site* site = event->site;
    // 1. Check if the bachline is empty
    if(mBeachline.isEmpty())
    {
        mBeachline.setRoot(new Node(site));
        return;
    }
    // 2. Look for the arc above the site
    Node* arcToBreak = mBeachline.locateArcAbove(site->point, mBeachlineY);
    deleteEvent(arcToBreak);
    // 3. Replace this arc by the new arcs
    Node* middleArc = breakArc(arcToBreak, site);
    Node* leftArc = middleArc->prev; 
    Node* rightArc = middleArc->next;
    // 4. Add an edge in the diagram
    addEdge(leftArc, middleArc);
    middleArc->rightHalfEdge = middleArc->leftHalfEdge;
    rightArc->leftHalfEdge = leftArc->rightHalfEdge;
    // 5. Check circle events
    // Left triplet
    if(leftArc->prev != nullptr)
        addEvent(leftArc->prev, leftArc, middleArc);
    // Right triplet
    if(rightArc->next != nullptr)
        addEvent(middleArc, rightArc, rightArc->next);
}

void FortuneAlgorithm::handleCircleEvent(Event* event)
{
    Vector2f point = event->point;
    Node* arc = event->arc;
    // 1. Add vertex
    const VoronoiDiagram::Vertex* vertex = mDiagram.createVertex(point);
    // 2. Delete all the events with this arc
    Node* leftArc = arc->prev;
    Node* rightArc = arc->next;
    deleteEvent(leftArc);
    deleteEvent(rightArc);
    // 3. Update the beachline and the diagram
    removeArc(arc, vertex);
    // 4. Add new circle events
    // Left triplet
    if(leftArc->prev != nullptr)
        addEvent(leftArc->prev, leftArc, rightArc);
    // Right triplet
    if(rightArc->next != nullptr)
        addEvent(leftArc, rightArc, rightArc->next);
}

Node* FortuneAlgorithm::breakArc(Node* arc, const VoronoiDiagram::Site* site)
{
    // Create the new subtree
    Node* middleArc = new Node(site);
    Node* leftArc = new Node(arc->site);
    leftArc->leftHalfEdge = arc->leftHalfEdge;
    Node* rightArc = new Node(arc->site);
    rightArc->rightHalfEdge = arc->rightHalfEdge;
    // Insert the subtree in the beachline
    mBeachline.replaceNode(arc, middleArc);
    mBeachline.insertBefore(middleArc, leftArc);
    mBeachline.insertAfter(middleArc, rightArc);
    // Delete old arc
    delete arc;
    // Return the root of the subtree
    return middleArc;
}

void FortuneAlgorithm::removeArc(Node* arc, const VoronoiDiagram::Vertex* vertex)
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

bool FortuneAlgorithm::isMovingRight(const Node* left, const Node* right) const
{
    return left->site->point.y < right->site->point.y;
}

float FortuneAlgorithm::getInitialX(const Node* left, const Node* right, bool movingRight)
{
    return movingRight ? left->site->point.x : right->site->point.x;
}

void FortuneAlgorithm::addEdge(Node* left, Node* right)
{
    // Create two new half edges
    left->rightHalfEdge = mDiagram.createHalfEdge(left->site->face);
    right->leftHalfEdge = mDiagram.createHalfEdge(right->site->face);
    // Set the two half edges twins
    left->rightHalfEdge->twin = right->leftHalfEdge;
    right->leftHalfEdge->twin = left->rightHalfEdge;
}

void FortuneAlgorithm::setOrigin(Node* left, Node* right, const VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->destination = vertex;
    right->leftHalfEdge->origin = vertex;
}

void FortuneAlgorithm::setDestination(Node* left, Node* right, const VoronoiDiagram::Vertex* vertex)
{
    left->rightHalfEdge->origin = vertex;
    right->leftHalfEdge->destination = vertex;
}

void FortuneAlgorithm::setPrevHalfEdge(VoronoiDiagram::HalfEdge* prev, VoronoiDiagram::HalfEdge* next)
{
    prev->next = next;
    next->prev = prev;
}

void FortuneAlgorithm::addEvent(Node* left, Node* middle, Node* right)
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
    if(isValid && isBelow)
    {
        Event* event = new Event(y, convergencePoint, middle);
        middle->event = event;
        mEvents.push(event);
    }
}

void FortuneAlgorithm::deleteEvent(Node* arc)
{
    if(arc->event != nullptr)
    {
        mEvents.remove(arc->event->index);
        delete arc->event;
        arc->event = nullptr;
    }
}

Vector2f FortuneAlgorithm::computeConvergencePoint(Vector2f point1, Vector2f point2, Vector2f point3, float& y)
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

void FortuneAlgorithm::free(Node* node)
{
    if(node == nullptr)
        return;
    else if(node->isLeaf())
        delete node;
    else
    {
        free(node->left);
        free(node->right);
        delete node;
    }
}

