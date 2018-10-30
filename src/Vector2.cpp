#include "Vector2.h"

Vector2f::Vector2f(float x, float y) : x(x), y(y)
{

}

// Unary operators

Vector2f Vector2f::operator-() const
{
    return Vector2f(-x, -y);
}

Vector2f& Vector2f::operator+=(const Vector2f& other)
{
    x += other.x;
    y += other.y;
    return *this;
}

Vector2f& Vector2f::operator-=(const Vector2f& other)
{
    x -= other.x;
    y -= other.y;
    return *this;
}

Vector2f& Vector2f::operator*=(float t)  
{
    x *= t;
    y *= t;
    return *this; 
}

// Other operations

Vector2f Vector2f::getOrthogonal() const
{
    return Vector2f(-y, x);
}

float Vector2f::dot(const Vector2f& other) const
{
    return x * other.x + y * other.y;
}

float Vector2f::getNorm() const
{
    return std::sqrt(x * x + y * y);
}

float Vector2f::getDistance(const Vector2f& other) const
{
    return (*this - other).getNorm();
}

float Vector2f::getDet(const Vector2f& other) const
{
    return x * other.y - y * other.x;
}

Vector2f operator+(Vector2f lhs, const Vector2f& rhs)
{
    lhs += rhs;
    return lhs;
}

Vector2f operator-(Vector2f lhs, const Vector2f& rhs) 
{
    lhs -= rhs;
    return lhs;
}

Vector2f operator*(float t, Vector2f vec)
{
    vec *= t;
    return vec;
}

Vector2f operator*(Vector2f vec, float t)
{
    return t * vec;
}

std::ostream& operator<<(std::ostream& os, const Vector2f& vec)
{
    os << "(" << vec.x << ", " << vec.y << ")";
    return os;
}

