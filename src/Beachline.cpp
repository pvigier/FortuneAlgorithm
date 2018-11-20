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

#include "Beachline.h"
// STL
#include <limits>
#include <cmath>
// My includes
#include "Arc.h"

Beachline::Beachline() : mNil(new Arc), mRoot(mNil)
{
    mNil->color = Arc::Color::BLACK; 
}

Beachline::~Beachline()
{
    free(mRoot);    
    delete mNil;
}

Arc* Beachline::createArc(VoronoiDiagram::Site* site)
{
    return new Arc{mNil, mNil, mNil, site, nullptr, nullptr, nullptr, mNil, mNil, Arc::Color::RED};
}

bool Beachline::isEmpty() const
{
    return isNil(mRoot);
}

bool Beachline::isNil(const Arc* x) const
{
    return x == mNil;
}

void Beachline::setRoot(Arc* x)
{
    mRoot = x;
    mRoot->color = Arc::Color::BLACK;
}

Arc* Beachline::getLeftmostArc() const
{
    Arc* x = mRoot;
    while (!isNil(x->prev))
        x = x->prev;
    return x;
}

Arc* Beachline::locateArcAbove(const Vector2& point, double l) const
{
    Arc* node = mRoot;
    bool found = false;
    while (!found)
    {
        double breakpointLeft = -std::numeric_limits<double>::infinity();
        double breakpointRight = std::numeric_limits<double>::infinity();
        if (!isNil(node->prev))
           breakpointLeft =  computeBreakpoint(node->prev->site->point, node->site->point, l);
        if (!isNil(node->next))
            breakpointRight = computeBreakpoint(node->site->point, node->next->site->point, l);
        if (point.x < breakpointLeft)
            node = node->left;
        else if (point.x > breakpointRight)
            node = node->right;
        else
            found = true;
    }
    return node;
}

void Beachline::insertBefore(Arc* x, Arc* y)
{
    // Find the right place
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
    // Set the pointers
    y->prev = x->prev;
    if (!isNil(y->prev))
        y->prev->next = y;
    y->next = x;
    x->prev = y;
    // Balance the tree
    insertFixup(y);    
}

void Beachline::insertAfter(Arc* x, Arc* y)
{
    // Find the right place
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
    // Set the pointers
    y->next = x->next;
    if (!isNil(y->next))
        y->next->prev = y;
    y->prev = x;
    x->next = y;
    // Balance the tree
    insertFixup(y);    
}

void Beachline::replace(Arc* x, Arc* y)
{
    transplant(x, y);
    y->left = x->left;
    y->right = x->right;
    if (!isNil(y->left))
        y->left->parent = y;
    if (!isNil(y->right))
        y->right->parent = y;
    y->prev = x->prev;
    y->next = x->next;
    if (!isNil(y->prev))
        y->prev->next = y;
    if (!isNil(y->next))
        y->next->prev = y;
    y->color = x->color;
}

void Beachline::remove(Arc* z)
{
    Arc* y = z;
    Arc::Color yOriginalColor = y->color;
    Arc* x;
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
    if (yOriginalColor == Arc::Color::BLACK)
        removeFixup(x);
    // Update next and prev
    if (!isNil(z->prev))
        z->prev->next = z->next;
    if (!isNil(z->next))
        z->next->prev = z->prev;
}

std::ostream& Beachline::print(std::ostream& os) const
{
    //return printArc(os, mRoot);
    Arc* arc = getLeftmostArc();
    while (!isNil(arc))
    {
        os << arc->site->index << ' ';
        arc = arc->next;
    }
    return os;
}

Arc* Beachline::minimum(Arc* x) const
{
    while (!isNil(x->left))
        x = x->left;
    return x;
}

void Beachline::transplant(Arc* u, Arc* v)
{
    if (isNil(u->parent))
        mRoot = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    v->parent = u->parent;
}

void Beachline::insertFixup(Arc* z)
{
    while (z->parent->color == Arc::Color::RED)
    {
        if (z->parent == z->parent->parent->left)
        {
            Arc* y = z->parent->parent->right;
            // Case 1
            if (y->color == Arc::Color::RED)
            {
                z->parent->color = Arc::Color::BLACK;
                y->color = Arc::Color::BLACK;
                z->parent->parent->color = Arc::Color::RED;
                z = z->parent->parent;
            }
            else
            {
                // Case 2
                if (z == z->parent->right)
                {
                    z = z->parent;
                    leftRotate(z);
                }
                // Case 3
                z->parent->color = Arc::Color::BLACK;
                z->parent->parent->color = Arc::Color::RED;
                rightRotate(z->parent->parent);
            }
        }
        else
        {
            Arc* y = z->parent->parent->left;
            // Case 1
            if (y->color == Arc::Color::RED)
            {
                z->parent->color = Arc::Color::BLACK;
                y->color = Arc::Color::BLACK;
                z->parent->parent->color = Arc::Color::RED;
                z = z->parent->parent;
            }
            else
            {
                // Case 2
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rightRotate(z);
                }
                // Case 3
                z->parent->color = Arc::Color::BLACK;
                z->parent->parent->color = Arc::Color::RED;
                leftRotate(z->parent->parent);
            }
        }
    }
    mRoot->color = Arc::Color::BLACK;
}

void Beachline::removeFixup(Arc* x)
{

    while (x != mRoot && x->color == Arc::Color::BLACK)
    {
        Arc* w;
        if (x == x->parent->left)
        {
            w = x->parent->right;
            // Case 1
            if (w->color == Arc::Color::RED)
            {
                w->color = Arc::Color::BLACK;
                x->parent->color = Arc::Color::RED;
                leftRotate(x->parent);
                w = x->parent->right;
            }
            // Case 2
            if (w->left->color == Arc::Color::BLACK && w->right->color == Arc::Color::BLACK)
            {
                w->color = Arc::Color::RED;
                x = x->parent;
            }
            else
            {
                // Case 3
                if (w->right->color == Arc::Color::BLACK)
                {
                    w->left->color = Arc::Color::BLACK;
                    w->color = Arc::Color::RED;
                    rightRotate(w);
                    w = x->parent->right;
                }
                // Case 4
                w->color = x->parent->color;
                x->parent->color = Arc::Color::BLACK;
                w->right->color = Arc::Color::BLACK;
                leftRotate(x->parent);
                x = mRoot;
            }
        }
        else
        {
            w = x->parent->left;
            // Case 1
            if (w->color == Arc::Color::RED)
            {
                w->color = Arc::Color::BLACK;
                x->parent->color = Arc::Color::RED;
                rightRotate(x->parent);
                w = x->parent->left;
            }
            // Case 2
            if (w->left->color == Arc::Color::BLACK && w->right->color == Arc::Color::BLACK)
            {
                w->color = Arc::Color::RED;
                x = x->parent;
            }
            else
            {
                // Case 3
                if (w->left->color == Arc::Color::BLACK)
                {
                    w->right->color = Arc::Color::BLACK;
                    w->color = Arc::Color::RED;
                    leftRotate(w);
                    w = x->parent->left;
                }
                // Case 4
                w->color = x->parent->color;
                x->parent->color = Arc::Color::BLACK;
                w->left->color = Arc::Color::BLACK;
                rightRotate(x->parent);
                x = mRoot;
            } 
        }
    }
    x->color = Arc::Color::BLACK;
}

void Beachline::leftRotate(Arc* x)
{
    Arc* y = x->right;
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

void Beachline::rightRotate(Arc* y)
{
    Arc* x = y->left;
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

double Beachline::computeBreakpoint(const Vector2& point1, const Vector2& point2, double l) const
{
    double x1 = point1.x, y1 = point1.y, x2 = point2.x, y2 = point2.y;
    double d1 = 1.0 / (2.0 * (y1 - l));
	double d2 = 1.0 / (2.0 * (y2 - l));
	double a = d1 - d2;
	double b = 2.0 * (x2 * d2 - x1 * d1);
	double c = (y1 * y1 + x1 * x1 - l * l) * d1 - (y2 * y2 + x2 * x2 - l * l) * d2;
	double delta = b * b - 4.0 * a * c;
    return (-b + std::sqrt(delta)) / (2.0 * a);
}

void Beachline::free(Arc* x)
{
    if (isNil(x))
        return;
    else
    {
        free(x->left);
        free(x->right);
        delete x;
    }
}

std::ostream& Beachline::printArc(std::ostream& os, const Arc* arc, std::string tabs) const
{
    os << tabs << arc->site->index << ' ' << arc->leftHalfEdge << ' ' << arc->rightHalfEdge << std::endl;
    if (!isNil(arc->left))
        printArc(os, arc->left, tabs + '\t');
    if (!isNil(arc->right))
        printArc(os, arc->right, tabs + '\t');
    return os;
}

std::ostream& operator<<(std::ostream& os, const Beachline& beachline)
{
    return beachline.print(os);
}
