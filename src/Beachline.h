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

// My includes
#include "Vector2.h"
#include "VoronoiDiagram.h"

class Arc;

class Beachline
{
public:
    Beachline();
    ~Beachline();

    // Remove copy and move operations
    Beachline(const Beachline&) = delete;
    Beachline& operator=(const Beachline&) = delete;
    Beachline(Beachline&&) = delete;
    Beachline& operator=(Beachline&&) = delete;

    Arc* createArc(VoronoiDiagram::Site* site);
    
    bool isEmpty() const;
    bool isNil(const Arc* x) const;
    void setRoot(Arc* x);
    Arc* getLeftmostArc() const;

    Arc* locateArcAbove(const Vector2& point, double l) const;
    void insertBefore(Arc* x, Arc* y);
    void insertAfter(Arc* x, Arc* y);
    void replace(Arc* x, Arc* y);
    void remove(Arc* z);

    std::ostream& print(std::ostream& os) const;

private:
    Arc* mNil;
    Arc* mRoot;

    // Utility methods
    Arc* minimum(Arc* x) const;
    void transplant(Arc* u, Arc* v); 

    // Fixup functions
    void insertFixup(Arc* z);
    void removeFixup(Arc* x);

    // Rotations
    void leftRotate(Arc* x);
    void rightRotate(Arc* y);

    double computeBreakpoint(const Vector2& point1, const Vector2& point2, double l) const;

    void free(Arc* x);

    std::ostream& printArc(std::ostream& os, const Arc* arc, std::string tabs = "") const;
};

std::ostream& operator<<(std::ostream& os, const Beachline& beachline);
