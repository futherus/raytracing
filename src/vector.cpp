#include "vector.h"
#include <iostream>

std::ostream& operator<<(std::ostream& stream, const vector_t& vec)
{
    return stream << "(" << vec.x_ << ", " << vec.y_  << ", " << vec.z_ << ")";
}

vector_t& operator+=(vector_t& a, const vector_t& b)
{
    a.x_ += b.x_;
    a.y_ += b.y_;
    a.z_ += b.z_;

    return a;
}

vector_t& operator-=(vector_t& a, const vector_t& b)
{
    a.x_ -= b.x_;
    a.y_ -= b.y_;
    a.z_ -= b.z_;

    return a;
}

vector_t& operator*=(vector_t& a, float scale)
{
    a.x_ *= scale;
    a.y_ *= scale;
    a.z_ *= scale;

    return a;
}

vector_t& operator*=(vector_t& a, const vector_t& b)
{
    a.x_ *= b.x_;
    a.y_ *= b.y_;
    a.z_ *= b.z_;

    return a;
}

vector_t operator+(vector_t a, const vector_t& b)
{
    a += b;
    return a;
}

vector_t operator-(vector_t a, const vector_t& b)
{
    a -= b;
    return a;
}

vector_t operator*(vector_t vec, float scale)
{
    vec *= scale;
    return vec;
}

vector_t operator*(float scale, vector_t vec)
{
    vec *= scale;
    return vec;
}

vector_t operator*(vector_t a, const vector_t& b)
{
    a *= b;
    return a;
}

float dot(const vector_t& a, const vector_t& b)
{
    return a.x_ * b.x_ + a.y_ * b.y_ + a.z_ * b.z_;
}

vector_t cross(const vector_t& a, const vector_t& b)
{
    vector_t tmp = 
    {
        a.y_ * b.z_ - a.z_ * b.y_,
        a.z_ * b.x_ - a.x_ * b.z_,
        a.x_ * b.y_ - a.y_ * b.x_
    };

    return tmp;
}
