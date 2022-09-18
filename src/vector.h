#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SFML/Graphics.hpp>
#include <iostream>

// class point_t
// {
//     float x_ = 0;
//     float y_ = 0;

// public:
//     point_t(float x, float y)
//         : x_(x), y_(y)
//     {}

//     point_t()
//         : x_(0), y_(0)
//     {}

//     float x() { return x_; }
//     float y() { return y_; }
// };

class vector_t
{
private:
    float x_;
    float y_;

public:
    vector_t(float x, float y)
        : x_(x), y_(y)
    {}

    vector_t()
        :x_(0), y_(0)
    {}

    vector_t(const vector_t& vec)
        :x_(vec.x_), y_(vec.y_)
    {}

    vector_t& operator= (const vector_t& vec)
    {
        x_ = vec.x_;
        y_ = vec.y_;

        return *this;
    }

    friend vector_t operator+(const vector_t&, const vector_t&);
    friend vector_t operator-(const vector_t&, const vector_t&);
    friend vector_t operator*(const vector_t&, float);
    friend vector_t operator*(float, const vector_t&);

    friend vector_t& operator+=(vector_t&, const vector_t&);
    friend vector_t& operator-=(vector_t&, const vector_t&);
    friend vector_t& operator*=(vector_t&, float);

    friend std::ostream& operator<<(std::ostream& stream, const vector_t& vec);

    float& x() { return x_; }
    float& y() { return y_; }

    float x() const { return x_; }
    float y() const { return y_; }
};

#endif // GRAPHICS_H
