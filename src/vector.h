#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>

class point_t
{
public:
    uint32_t x_ = 0;
    uint32_t y_ = 0;

    point_t(uint32_t x, uint32_t y)
        : x_(x), y_(y)
    {}

    point_t()
        : x_(0), y_(0)
    {}
};

class vector_t
{
public:
    float x_;
    float y_;
    float z_;

    vector_t(const vector_t& vec)            = default;
    vector_t& operator=(const vector_t& vec) = default;
    ~vector_t()                              = default;

    constexpr vector_t(float x, float y, float z)
        : x_(x), y_(y), z_(z)
    {}

    constexpr vector_t()
        :x_(0), y_(0), z_(0)
    {}

    vector_t& operator+=(const vector_t&);
    vector_t& operator-=(const vector_t&);
    vector_t& operator*=(const vector_t&);
    vector_t& operator*=(float);
    vector_t& operator/=(float);

    bool is_zero() const;
    void saturate(float);

    friend vector_t operator+(vector_t, const vector_t&);
    friend vector_t operator-(vector_t, const vector_t&);
    friend vector_t operator*(vector_t, const vector_t&);
    friend vector_t operator*(vector_t, float);
    friend vector_t operator*(float, vector_t);
    friend vector_t operator/(vector_t, float);
    friend vector_t operator-(vector_t);
    friend vector_t operator+(vector_t);

    friend float    dot  (const vector_t&, const vector_t&);
    friend vector_t cross(const vector_t&, const vector_t&);

    friend std::ostream& operator<<(std::ostream& stream, const vector_t& vec);
};

#endif // VECTOR_H
