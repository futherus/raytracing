#include <cmath>
#include <assert.h>
#include "scene.h"

vector_t scene_t::eval_color(const vector_t& point)
{
    vector_t delta = point - ball_.pos();
    
    float z_sq = ball_.radius() * ball_.radius() - dot(delta, delta);

    if (z_sq < 0)
        return bckgrnd_col_;

    vector_t color = ambient_col_ * ball_.col();

    delta.z_ = std::sqrt(z_sq);
    vector_t norm = delta * (1 / ball_.radius());

    float cos_incid = dot(norm, light_.dir()) / dot(light_.dir(), light_.dir());
    if (cos_incid < 0)
        cos_incid = 0;

    color += cos_incid * light_.col() * ball_.col();

    if (color.x_ > 1.f)
        color.x_ = 1.f;
    if (color.y_ > 1.f)
        color.y_ = 1.f;
    if (color.z_ > 1.f)
        color.z_ = 1.f;

    return color;
}