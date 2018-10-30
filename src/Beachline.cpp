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
    mRoot->color = Node::Color::BLACK;
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
    // Balance the tree
    insertFixup(y);    
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
    // Balance the tree
    insertFixup(y);    
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
    newNode->color = oldNode->color;
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
    while (z->parent->color == Node::Color::RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            Node* y = z->parent->parent->right;
            if (y->color == Node::Color::RED)
            {
                z->parent->color = Node::Color::BLACK;
                y->color = Node::Color::BLACK;
                z->parent->parent->color = Node::Color::RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(z);
                }
                z->parent->color = Node::Color::BLACK;
                z->parent->parent->color = Node::Color::RED;
                rightRotate(z->parent->parent);
            }
        }
        else
        {
            Node* y = z->parent->parent->left;
            if (y->color == Node::Color::RED)
            {
                z->parent->color = Node::Color::BLACK;
                y->color = Node::Color::BLACK;
                z->parent->parent->color = Node::Color::RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(z);
                }
                z->parent->color = Node::Color::BLACK;
                z->parent->parent->color = Node::Color::RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    mRoot->color = Node::Color::BLACK;
}

void Beachline::removeFixup(Node* x)
{

    while (x != mRoot && x->color == Node::Color::BLACK)
    {
        Node* w;
        if (x == x->parent->left)
        {
            w = x->parent->right;
            if (w->color == Node::Color::RED)
            {
                w->color = Node::Color::BLACK;
                x->parent->color = Node::Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            if (w->left->color == Node::Color::BLACK && w->right->color == Node::Color::BLACK)
            {
                w->color = Node::Color::RED;
                x = x->parent;
            }
            else
            {
                if (w->right->color == Node::Color::BLACK)
                {
                    w->left->color = Node::Color::BLACK;
                    w->color = Node::Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = Node::Color::BLACK;
                w->right->color = Node::Color::BLACK;
                leftRotate(x->parent);
                x = mRoot;
            }
        }
        else
        {
            w = x->parent->left;
            if (w->color == Node::Color::RED)
            {
                w->color = Node::Color::BLACK;
                x->parent->color = Node::Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            if (w->left->color == Node::Color::BLACK && w->right->color == Node::Color::BLACK)
            {
                w->color = Node::Color::RED;
                x = x->parent;
            }
            else
            {
                if (w->left->color == Node::Color::BLACK)
                {
                    w->right->color = Node::Color::BLACK;
                    w->color = Node::Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = Node::Color::BLACK;
                w->left->color = Node::Color::BLACK;
                rightRotate(x->parent);
                x = mRoot;
            } 
        }
    }
    x->color = Node::Color::BLACK;
}

void Beachline::leftRotate(Node* x)
{
    Node* y = x->right;
    x->right = y->left;
    if (!isNil(y->left))
        y->left->parent = x;
    y->parent = x->parent;
    if (isNil(x->parent))
        mRoot = y;
    else if (x->parent->left == x)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void Beachline::rightRotate(Node* y)
{
    Node* x = y->left;
    y->left = x->right;
    if (!isNil(x->right))
        x->right->parent = y;
    x->parent = y->parent;
    if (isNil(y->parent))
        mRoot = x;
    else if (y->parent->left == y)
        y->parent->left = x;
    else
        y->parent->right = x;
    x->right = y;
    y->parent = x;
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

