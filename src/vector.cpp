#include "vector.h"
#include <iostream>

vector_t& vector_t::operator+=(const vector_t& b)
{
    x_ += b.x_;
    y_ += b.y_;
    z_ += b.z_;

    return *this;
}

vector_t& vector_t::operator-=(const vector_t& b)
{
    x_ -= b.x_;
    y_ -= b.y_;
    z_ -= b.z_;

    return *this;
}

vector_t& vector_t::operator*=(const vector_t& b)
{
    x_ *= b.x_;
    y_ *= b.y_;
    z_ *= b.z_;

    return *this;
}

vector_t& vector_t::operator*=(float scale)
{
    x_ *= scale;
    y_ *= scale;
    z_ *= scale;

    return *this;
}

vector_t& vector_t::operator/=(float scale)
{
    x_ /= scale;
    y_ /= scale;
    z_ /= scale;

    return *this;
}

bool vector_t::is_zero() const
{
    float vec_sq = x_ * x_ + y_ * y_ + z_ * z_;
    // FIXME need to find better solution
    return vec_sq < __FLT_EPSILON__;
}

void vector_t::saturate(float lim)
{
    if (x_ > lim)
        x_ = lim;
    if (y_ > lim)
        y_ = lim;
    if (z_ > lim)
        z_ = lim;
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

vector_t operator*(vector_t a, const vector_t& b)
{
    a *= b;
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

vector_t operator/(vector_t vec, float scale)
{
    vec /= scale;
    return vec;
}

vector_t operator-(vector_t vec)
{
    return (-1) * vec;
}

vector_t operator+(vector_t vec)
{
    return vec;
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

std::ostream& operator<<(std::ostream& stream, const vector_t& vec)
{
    return stream << "(" << vec.x_ << ", " << vec.y_  << ", " << vec.z_ << ")";
}
