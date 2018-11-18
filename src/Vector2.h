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
#include <ostream>

// Declarations

class Vector2;
Vector2 operator-(Vector2 lhs, const Vector2& rhs);

// Implementations

class Vector2
{
public:
    double x;
    double y;

    Vector2(double x = 0.0, double y = 0.0);
    
    // Unary operators

    Vector2 operator-() const;
    Vector2& operator+=(const Vector2& other);
    Vector2& operator-=(const Vector2& other);
    Vector2& operator*=(double t);
    
    // Other operations
    
    Vector2 getOrthogonal() const;
    double dot(const Vector2& other) const;
    double getNorm() const;
    double getDistance(const Vector2& other) const;
    double getDet(const Vector2& other) const;
};

// Binary operators

Vector2 operator+(Vector2 lhs, const Vector2& rhs);
Vector2 operator-(Vector2 lhs, const Vector2& rhs);
Vector2 operator*(double t, Vector2 vec);
Vector2 operator*(Vector2 vec, double t);
std::ostream& operator<<(std::ostream& os, const Vector2& vec);

