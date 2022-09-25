#ifndef AXES_H
#define AXES_H

#include <iostream>
#include <assert.h>

#include "vector.h"

class axes_t
{
private:
    point_t tl_pos_;
    point_t br_pos_;

    vector_t min_lim_;
    vector_t max_lim_;

public:
    axes_t(point_t  tl_pos, point_t  br_pos,
           vector_t min_lim, vector_t max_lim)
        : tl_pos_(tl_pos), br_pos_(br_pos), min_lim_(min_lim), max_lim_(max_lim)
    {
        assert(min_lim.z_ == 0.f && max_lim.z_ == 0.f);
    }

    vector_t pixel2real(point_t pixel);
    // point_t real2pixel(vector_t real);
};

#endif // AXES_H
