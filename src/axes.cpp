#include <assert.h>
#include "axes.h"

// vector_t axes_t::real2pixel(vector_t real)
// {
//     assert(min_lim_.x_ <= real.x_ && real.x_ <= max_lim_.x_);
//     assert(min_lim_.y_ <= real.y_ && real.y_ <= max_lim_.y_);

//     float s11 =   (br_pos_.x_ - tl_pos_.x_) / (max_lim_.x_ - min_lim_.x_);
//     float s22 = - (br_pos_.y_ - tl_pos_.y_) / (max_lim_.y_ - min_lim_.y_);

//     vector_t origin(-min_lim_.x_ * s11, -max_lim_.y_ * s22, 0);

//     return vector_t(s11 * real.x_, s22 * real.y_, 0) + origin + tl_pos_;
// }

vector_t axes_t::pixel2real(point_t pixel) const
{
    assert(pixel.x_ <= sz_pixel_.x_);
    assert(pixel.y_ <= sz_pixel_.y_);

    vector_t real = tl_pos_;
    
    real += x_side_ * ((float) pixel.x_ / (float) sz_pixel_.x_);
    real += y_side_ * ((float) pixel.y_ / (float) sz_pixel_.y_);

    return real;
}
