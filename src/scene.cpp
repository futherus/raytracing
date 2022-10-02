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

bool ball_t::is_intersection(float& dist, const ray_t& ray) const
{
    vector_t delta = pos() - ray.pos();

    // dir is unit
    float project = dot(delta, ray.dir());
    if (project < 0)
        return false;

    vector_t distance = ray.dir() * project - delta;

    float distance_sq = dot(distance, distance);
    float rad_sq = radius() * radius();
    if (distance_sq >= rad_sq)
        return false;

    // dist^2 + (k*dir)^2 = r^2, r^2 > dist^2
    float k = std::sqrt(rad_sq - distance_sq);

    // dir is unit
    dist = project - k;
    // if (!(dist > -__FLT_EPSILON__))
    //     fprintf(stderr, "Ray emitted inside object? %lg\n", dist);

    return true;
}

vector_t ball_t::intersect(const ray_t& ray) const
{
    vector_t delta = pos() - ray.pos();

    // dir is unit
    float project = dot(delta, ray.dir());
    vector_t distance = ray.dir() * project - delta;

    float distance_sq = dot(distance, distance);
    float rad_sq = radius() * radius();

    // n^2 + (k*dir)^2 = r^2, r^2 > n^2
    float k = std::sqrt(rad_sq - distance_sq);
    vector_t rad = distance - k * ray.dir();

    vector_t normal  = rad / radius();
    vector_t point   = pos() + rad;
    vector_t reflect = 2 * normal * dot(-ray.dir(), normal) - (-ray.dir()); 

    ray_t reflect_ray(ray, point, reflect);
    vector_t color = this->color() * reflect_ray.emit();
    color.saturate(1.f);

    return color;
}

bool light_t::is_intersection(float& dist, const ray_t& ray) const
{
    vector_t delta = pos() - ray.pos();

    // dir is unit
    float project = dot(delta, ray.dir());
    if (project < 0)
        return false;

    vector_t distance = ray.dir() * project - delta;

    float distance_sq = dot(distance, distance);
    float rad_sq = radius() * radius();
    if (distance_sq >= rad_sq)
        return false;

    // n^2 + (k*dir)^2 = r^2, r^2 > n^2
    float k = std::sqrt(rad_sq - distance_sq);

    // dir is unit
    dist = project - k;
    // if (!(dist > -__FLT_EPSILON__))
    //     fprintf(stderr, "Ray emitted inside object? %lg\n", dist);

    return true;
}

vector_t light_t::intersect(const ray_t&) const
{
    return color();
}

vector_t ray_t::emit() const
{
    if (heir_depth_ == 0)
        return parent_->ambient_col_;

    assert(heir_depth_ > 0);

    vector_t color;

    float dist     = 0.f;
    float min_dist = 0.f;
    size_t index   = (size_t) -1;

    for (size_t i = 0; i < parent_->objects_.size(); i++)
    {
        if (parent_->objects_[i]->is_intersection(dist, *this))
        {
            if (dist < min_dist || index == (size_t) -1)
            {
                min_dist = dist;
                index = i;
            }
        }
    }

    if (index != (size_t) -1)
        return parent_->objects_[index]->intersect(*this);

    // std::cout << "Ambient emit\n";

    return parent_->ambient_col_;
}

vector_t scene_t::eval_color(const point_t& pixel) const
{
    ray_t initial(this, 5);

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

    vector_t color = ambient_col_ * ball_.color();

    delta.z_ = std::sqrt(z_sq);
    vector_t norm = delta * (1 / ball_.radius());

    // norm and light_.dir() are normalised
    float cos_incid = dot(norm, light_.dir());
    if (cos_incid < 0)
        cos_incid = 0;

    color += cos_incid * light_.color() * ball_.color();

    vector_t pr_light_norm = norm * dot(light_.dir(), norm);
    vector_t refl = 2 * pr_light_norm - light_.dir();
    refl /= std::sqrt(dot(refl, refl));

    vector_t view(0, 0, 1);

    float cos_specul = dot(view, refl);
    if (cos_specul <= 0)
        cos_specul = 0;

    color += pow(cos_specul, ball_.glare()) * light_.color();

    if (color.x_ > 1.f)
        color.x_ = 1.f;
    if (color.y_ > 1.f)
        color.y_ = 1.f;
    if (color.z_ > 1.f)
        color.z_ = 1.f;

    return color;
}

*/
