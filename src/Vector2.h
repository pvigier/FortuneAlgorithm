#pragma once

#include <ostream>
#include <cmath>

// Declarations

class Vector2f;
Vector2f operator-(Vector2f lhs, const Vector2f& rhs);

// Implementations

class Vector2f
{
public:
    float x;
    float y;

    Vector2f(float x = 0.0f, float y = 0.0f);
    
    // Unary operators

    Vector2f operator-() const;
    Vector2f& operator+=(const Vector2f& other);
    Vector2f& operator-=(const Vector2f& other);
    Vector2f& operator*=(float t);
    
    // Other operations
    
    Vector2f getOrthogonal() const;
    float dot(const Vector2f& other) const;
    float getNorm() const;
    float getDistance(const Vector2f& other) const;
    float getDet(const Vector2f& other) const;
};

// Binary operators

Vector2f operator+(Vector2f lhs, const Vector2f& rhs);
Vector2f operator-(Vector2f lhs, const Vector2f& rhs);
Vector2f operator*(float t, Vector2f vec);
Vector2f operator*(Vector2f vec, float t);
std::ostream& operator<<(std::ostream& os, const Vector2f& vec);

