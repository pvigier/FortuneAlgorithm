#include "Beachline.h"
#include <limits>
#include "Node.h"

Beachline::Beachline() : mRoot(nullptr)
{
    //ctor
}

Beachline::~Beachline()
{
    //dtor
}

bool Beachline::isEmpty()
{
    return mRoot == nullptr;
}

Node* Beachline::getRoot()
{
    return mRoot;
}

void Beachline::setRoot(Node* root)
{
    mRoot = root;
}

Node* Beachline::locateArcAbove(Vector2f point, float l) const
{
    Node* node = mRoot;
    bool found = false;
    while(!found)
    {
        float breakpointLeft = -std::numeric_limits<float>::infinity();
        float breakpointRight = std::numeric_limits<float>::infinity();
        if (node->prev)
           breakpointLeft =  computeBreakpoint(node->prev->site->point, node->site->point, l);
        if (node->next)
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

void Beachline::replaceNode(Node* oldNode, Node* newNode)
{
    transplant(oldNode, newNode);
    newNode->left = oldNode->left;
    newNode->right = oldNode->right;
    if (newNode->left)
        newNode->left->parent = newNode;
    if (newNode->right)
        newNode->right->parent = newNode;
}

void Beachline::remove(Node* z)
{
    if (z->left == nullptr)
        transplant(z, z->right);
    else if (z->right == nullptr)
        transplant(z, z->left);
    else
    {
        Node* y = minimum(z->right);
        if (y->parent != z)
        {
            transplant(y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
    }
    // Update next and prev
    if (z->prev)
        z->prev->next = z->next;
    if (z->next)
        z->next->prev = z->prev;
}

Node* Beachline::minimum(Node* x)
{
    while (x->left != nullptr)
        x = x->left;
    return x;
}

void Beachline::display()
{
    mRoot->display("");
}

void Beachline::transplant(Node* u, Node* v)
{
    if (u->parent == nullptr)
        mRoot = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v != nullptr)
        v->parent = u->parent;
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

