#include "vector.h"
#include <iostream>

std::ostream& operator<<(std::ostream& stream, const vector_t& vec)
{
    return stream << "(" << vec.x_ << ", " << vec.y_ << ")";
}

vector_t& operator+=(vector_t& a, const vector_t& b)
{
    a.x() += b.x();
    a.y() += b.y();

    return a;
}

vector_t& operator-=(vector_t& a, const vector_t& b)
{
    a.x() -= b.x();
    a.y() -= b.y();

    return a;
}

vector_t& operator*=(vector_t& a, float scale)
{
    a.x() *= scale;
    a.y() *= scale;

    return a;
}

vector_t operator+(const vector_t& a, const vector_t& b)
{
    vector_t tmp = a;
    return tmp  += b;
}

vector_t operator-(const vector_t& a, const vector_t& b)
{
    vector_t tmp = a;
    return tmp  -= b;
}

vector_t operator*(const vector_t& vec, float scale)
{
    vector_t tmp = vec;
    return tmp *= scale;
}

vector_t operator*(float scale, const vector_t& vec)
{
    vector_t tmp = vec;
    return tmp *= scale;
}
