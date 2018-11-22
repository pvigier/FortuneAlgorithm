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
#include <array>
#include <limits>
// My includes
#include "Vector2.h"

class Box
{
public:
    // Be careful, y-axis is oriented to the top like in math
    enum class Side : int {LEFT, BOTTOM, RIGHT, TOP};

    struct Intersection
    {
        Side side;
        Vector2 point;
    };

    double left;
    double bottom;
    double right;
    double top;

    bool contains(const Vector2& point) const;
    Intersection getFirstIntersection(const Vector2& origin, const Vector2& direction) const; // Useful for Fortune's algorithm
    int getIntersections(const Vector2& origin, const Vector2& destination, std::array<Intersection, 2>& intersections) const; // Useful for diagram intersection

private:
    static constexpr double EPSILON = std::numeric_limits<double>::epsilon();
};

