#ifndef AXES_H
#define AXES_H

#include <iostream>
#include <assert.h>
#include <cmath>

#include "vector.h"

class axes_t
{
private:
    point_t sz_pixel_;

    vector_t tl_pos_;
    
    vector_t x_side_;
    vector_t y_side_;

public:
    axes_t(point_t sz_pixel, vector_t tl_pos, vector_t x_side, vector_t y_side)
        : sz_pixel_(sz_pixel), tl_pos_(tl_pos), x_side_(x_side), y_side_(y_side)
    {}

    vector_t pixel2real(point_t pixel) const;
    // point_t real2pixel(vector_t real);
};

#endif // AXES_H
