#include <cmath>
#include <random>
#include <assert.h>
#include "scene.h"

static std::default_random_engine* distr_engine()
{
    static std::random_device rdev{};
    static std::default_random_engine engine{rdev()};

    return &engine;
}

static float uniform_distr(float from, float to)
{
    static std::uniform_real_distribution<float> distr{};

    return distr(*distr_engine(), decltype(distr)::param_type{from, to});
}

static float normal_distr(float mean, float deviation)
{
    static std::normal_distribution<float> distr{};

    return distr(*distr_engine(), decltype(distr)::param_type{mean, deviation});
}

static void distribute_rays(std::vector<vector_t>& dirs, float (*distr_func)(float),
                            float begin, float end, float weight)
{
    if (distr_func(end) - distr_func(begin) < weight)
    {
        vector_t dir;
        dir.z_ = std::cos((begin + end) / 2.f);
        dirs.push_back(dir);
    }
    else
    {
        float avrg = (begin + end) / 2.f;
        distribute_rays(dirs, distr_func, begin, avrg, weight);
        distribute_rays(dirs, distr_func, avrg,  end,  weight);
    }
}

material_t::material_t(float (*distr_func)(float angle), int dirs_count,
                       vector_t glow, vector_t color)
    : dirs_(), basis_(), glow_(glow), color_(color)
{
    if (dirs_count == 0)
        return;

    dirs_.push_back(vector_t(0, 0, 1));

    if (distr_func == nullptr && dirs_count > 0)
        return;

    distribute_rays(dirs_, distr_func, 0, M_PI / 2.f, 1.f / dirs_count);

    float rot = 0.f;
    for (size_t i = 0; i < dirs_.size(); i++)
    {
        vector_t dir = dirs_.at(i);
        float radius = std::sqrt(1 - dir.z_ * dir.z_);
        dir.x_ = radius * std::cos(rot);
        dir.y_ = radius * std::sin(rot);

        dirs_.at(i) = dir;
        rot += uniform_distr(0, 2 * M_PI);

        std::cout << dirs_.at(i) << std::endl;
    }
}

void material_t::set_basis(const vector_t& normal)
{
    // assert(dot(normal, normal) == 1);

    basis_[2] = normal;

    if (normal.x_ != 0)
    {
        basis_[1].x_ = - (normal.y_ + normal.z_) / normal.x_;
        basis_[1].y_ = 1;
        basis_[1].z_ = 1;
    }
    else if (normal.y_ != 0)
    {
        basis_[1].x_ = 1;
        basis_[1].y_ = - (normal.x_ + normal.z_) / normal.y_;
        basis_[1].z_ = 1;
    }
    else if (normal.z_ != 0)
    {
        basis_[1].x_ = 1;
        basis_[1].y_ = 1;
        basis_[1].z_ = - (normal.x_ + normal.y_) / normal.z_;
    }

    basis_[1] /= std::sqrt(dot(basis_[1], basis_[1]));

    basis_[0] = cross(basis_[1], basis_[2]);
}

vector_t material_t::dir(size_t index) const
{
    vector_t dir = dirs_.at(index);

    return basis_[0] * dir.x_ + basis_[1] * dir.y_ + basis_[2] * dir.z_;
}

shape_t::~shape_t() {}

bool object_t::get_intersection(float& distance, vector_t& point, vector_t& normal,
                                const ray_t& ray) const
{
    return shape_->get_intersection(distance, point, normal, ray);
}

vector_t object_t::intersect(const vector_t& point, const vector_t& normal, const ray_t& ray) const
{
    assert(fabs(dot(normal, normal) - 1) < 0.001);
    vector_t reflect = 2 * normal * dot(-ray.dir(), normal) - (-ray.dir()); 

    material_->set_basis(reflect);

    vector_t color = material_->glow();
    for (size_t i = 0; i < material_->n_dir(); i++)
    {
        ray_t new_ray(ray, point, material_->dir(i));
        color += material_->color() * material_->weight() * new_ray.emit();
    }

    color.saturate(1.f);

    return color;
}

bool ball_t::get_intersection(float& dist, vector_t& point, vector_t& normal,
                              const ray_t& ray) const
{
    vector_t delta = pos() - ray.pos();

    assert(fabs(dot(ray.dir(), ray.dir()) - 1) < 0.001);

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
    // if (k > project)
    //     return false;

    vector_t rad = distance - k * ray.dir();

    normal = rad / radius();
    point  = pos() + rad;
    dist   = project - k;

    // if (!(dist > -__FLT_EPSILON__))
    //     fprintf(stderr, "Ray emitted inside object? %lg\n", dist);

    return true;
}

/*
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
*/

void camera_t::init_ray(ray_t& ray, point_t pixel) const
{
    vector_t pos = axes_.pixel2real(pixel);
    vector_t dir = pos - pov_;

    ray.set_pos(pos);
    ray.set_dir(dir);
}

vector_t ray_t::emit() const
{
    if (heir_depth_ == 0)
        return parent_->ambient_col_;

    assert(heir_depth_ > 0);

    float min_dist = 0.f;
    vector_t min_point;
    vector_t min_normal;
    size_t index   = (size_t) -1;

    for (size_t i = 0; i < parent_->objects_.size(); i++)
    {
        vector_t point;
        vector_t normal;
        float    dist = 0;

        if (parent_->objects_[i]->get_intersection(dist, point, normal, *this))
        {
            if (dist < min_dist || index == (size_t) -1)
            {
                min_dist   = dist;
                min_point  = point;
                min_normal = normal;
                index = i;
            }
        }
    }

    if (index != (size_t) -1)
        return parent_->objects_[index]->intersect(min_point, min_normal, *this);

    return parent_->ambient_col_;
}

vector_t scene_t::eval_color(const point_t& pixel) const
{
    ray_t initial(this, 5);

    camera_.init_ray(initial, pixel);

    return initial.emit();
}
