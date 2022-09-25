#include <cmath>
#include <assert.h>
#include "scene.h"

void camera_t::init_ray(ray_t& ray, point_t pixel) const
{
    vector_t pos = axes_.pixel2real(pixel);
    vector_t dir = pos - pov_;

    ray.set_pos(pos);
    ray.set_dir(dir);
}

bool ball_t::collision(vector_t& color, ray_t ray) const
{
    vector_t delta = pos() - ray.pos();

    // dir is unit
    vector_t normal = ray.dir() * dot(delta, ray.dir()) - delta;

    float norm_sq = dot(normal, normal);
    float rad_sq = radius() * radius();
    if (norm_sq > rad_sq)
        return false;
    
    // n^2 + k*dir^2 = r^2, r^2 > n^2
    float k = std::sqrt(rad_sq - norm_sq);
    vector_t point = pos() + normal - k * ray.dir();

    color = col();
    return true;
}

bool light_t::collision(vector_t& color, ray_t ray) const
{
    return false;
}

vector_t ray_t::emit() const
{
    vector_t color;
    if (heir_depth_ == 0)
        return color;

    if (parent_->ball_.collision(color, *this))
        return color;
    
    if (parent_->light_.collision(color, *this))
        return color;
    
    return color;
}

vector_t scene_t::eval_color(const point_t& pixel) const
{
    ray_t initial(this, 3);

    camera_.init_ray(initial, pixel);

    return initial.emit();
}

/*
vector_t scene_t::eval_color(const vector_t& point)
{
    vector_t delta = point - ball_.pos();
    
    float z_sq = ball_.radius() * ball_.radius() - dot(delta, delta);

    if (z_sq < 0)
        return bckgrnd_col_;

    vector_t color = ambient_col_ * ball_.col();

    delta.z_ = std::sqrt(z_sq);
    vector_t norm = delta * (1 / ball_.radius());

    // norm and light_.dir() are normalised
    float cos_incid = dot(norm, light_.dir());
    if (cos_incid < 0)
        cos_incid = 0;

    color += cos_incid * light_.col() * ball_.col();

    vector_t pr_light_norm = norm * dot(light_.dir(), norm);
    vector_t refl = 2 * pr_light_norm - light_.dir();
    refl /= std::sqrt(dot(refl, refl));

    vector_t view(0, 0, 1);

    float cos_specul = dot(view, refl);
    if (cos_specul <= 0)
        cos_specul = 0;

    color += pow(cos_specul, ball_.glare()) * light_.col();

    if (color.x_ > 1.f)
        color.x_ = 1.f;
    if (color.y_ > 1.f)
        color.y_ = 1.f;
    if (color.z_ > 1.f)
        color.z_ = 1.f;

    return color;
}

*/
