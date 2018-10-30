#include "Beachline.h"
#include <limits>
#include "Node.h"

Beachline::Beachline() : mNil(new Node), mRoot(mNil)
{
    mNil->color = Node::Color::BLACK; 
}

Beachline::~Beachline()
{
    free(mRoot);    
    delete mNil;
}

Node* Beachline::createNode(const VoronoiDiagram::Site* site)
{
    return new Node{mNil, mNil, mNil, site, nullptr, nullptr, nullptr, mNil, mNil, Node::Color::RED};
}

bool Beachline::isEmpty() const
{
    return isNil(mRoot);
}

bool Beachline::isNil(Node* x) const
{
    return x == mNil;
}

void Beachline::setRoot(Node* root)
{
    mRoot = root;
}

Node* Beachline::locateArcAbove(Vector2f point, float l) const
{
    Node* node = mRoot;
    bool found = false;
    while (!found)
    {
        float breakpointLeft = -std::numeric_limits<float>::infinity();
        float breakpointRight = std::numeric_limits<float>::infinity();
        if (!isNil(node->prev))
           breakpointLeft =  computeBreakpoint(node->prev->site->point, node->site->point, l);
        if (!isNil(node->next))
            breakpointRight = computeBreakpoint(node->site->point, node->next->site->point, l);
        if (point.x < breakpointLeft)
            node = node->prev;
        else if (point.x > breakpointRight)
            node = node->next;
        else
            found = true;
    }
    return node;
}

void Beachline::insertBefore(Node* x, Node* y)
{
    if (isNil(x->left))
    {
        x->left = y;
        y->parent = x;
    }
    else
    {
        x->prev->right = y;
        y->parent = x->prev;
    }
    y->prev = x->prev;
    if (!isNil(y->prev))
        y->prev->next = y;
    y->next = x;
    x->prev = y;
}

void Beachline::insertAfter(Node* x, Node* y)
{
    if (isNil(x->right))
    {
        x->right = y;
        y->parent = x;
    }
    else
    {
        x->next->left = y;
        y->parent = x->next;
    }
    y->next = x->next;
    if (!isNil(y->next))
        y->next->prev = y;
    y->prev = x;
    x->next = y;
}

void Beachline::replaceNode(Node* oldNode, Node* newNode)
{
    transplant(oldNode, newNode);
    newNode->left = oldNode->left;
    newNode->right = oldNode->right;
    if (!isNil(newNode->left))
        newNode->left->parent = newNode;
    if (!isNil(newNode->right))
        newNode->right->parent = newNode;
    newNode->prev = oldNode->prev;
    newNode->next = oldNode->next;
    if (!isNil(newNode->prev))
        newNode->prev->next = newNode;
    if (!isNil(newNode->next))
        newNode->next->prev = newNode;
}

void Beachline::remove(Node* z)
{
    Node* y = z;
    Node::Color yOriginalColor = y->color;
    Node* x;
    if (isNil(z->left))
    {
        x = z->right;
        transplant(z, z->right);
    }
    else if (isNil(z->right))
    {
        x = z->left;
        transplant(z, z->left);
    }
    else
    {
        y = minimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z)
            x->parent = y; // Because x could be Nil
        else
        {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }
    if (yOriginalColor == Node::Color::BLACK)
        removeFixup(x);
    // Update next and prev
    if (!isNil(z->prev))
        z->prev->next = z->next;
    if (!isNil(z->next))
        z->next->prev = z->prev;
}

void Beachline::display() const
{
    mRoot->display("");
}

Node* Beachline::minimum(Node* x) const
{
    while (!isNil(x->left))
        x = x->left;
    return x;
}

void Beachline::transplant(Node* u, Node* v)
{
    if (isNil(u->parent))
        mRoot = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void Beachline::insertFixup(Node* z)
{

}

void Beachline::removeFixup(Node* x)
{

}

float Beachline::computeBreakpoint(Vector2f point1, Vector2f point2, float l) const
{
    float x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;
    float d1 = 1 / (2 * (y1 - l));
	float d2 = 1 / (2 * (y2 - l));
	float a = d1 - d2;
	float b = 2 * (x2 * d2 - x1 * d1);
	float c = (y1 * y1 + x1 * x1 - l * l) * d1 - (y2 * y2 + x2 * x2 - l * l) * d2;
	float delta = b * b - 4 * a * c;
    return (-b + std::sqrt(delta)) / (2 * a);
}

void Beachline::free(Node* node)
{
    if (isNil(node))
        return;
    else
    {
        free(node->left);
        free(node->right);
        delete node;
    }
}

