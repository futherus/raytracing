#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "axes.h"
#include <cmath>
#include <vector>

class scene_t;
class ray_t;

//=============================================================================
class object_t
{
public:
    virtual bool is_intersection(float& distance, const ray_t& ray) const = 0;
    virtual vector_t intersect(const ray_t& ray) const = 0;

    virtual ~object_t() = 0;
};

inline object_t::~object_t() {};

class ball_t : public object_t
{
private:
    vector_t pos_;
    float radius_;

    vector_t col_;
    float glare_;

public:
    ball_t(const ball_t& ball)            = default;
    ball_t& operator=(const ball_t& ball) = default;
    ~ball_t()                             = default;

    ball_t(const vector_t& pos, float radius, const vector_t& col, float glare)
        : pos_(pos), radius_(radius), col_(col), glare_(glare)
    {}

    vector_t pos() const { return pos_; }
    float radius() const { return radius_; }
    vector_t color() const { return col_; }
    float glare()  const { return glare_; }

    void set_pos(const vector_t& new_pos) { pos_ = new_pos; }
    void set_radius(float new_radius) { radius_ = new_radius; }
    void set_col(const vector_t& new_col) { col_ = new_col; }
    void set_glare(float new_glare) { glare_ = new_glare; }

    bool is_intersection(float& distance, const ray_t& ray) const override;
    vector_t intersect(const ray_t& ray) const override;
};

class light_t : public object_t
{
private:
    vector_t pos_;
    float radius_;

    vector_t col_;

public:
    light_t(const light_t& ball)            = default;
    light_t& operator=(const light_t& ball) = default;
    ~light_t()                              = default;

    light_t(const vector_t& pos, float radius, const vector_t& col)
        : pos_(pos), radius_(radius), col_(col)
    {}

    vector_t pos() const { return pos_; }
    float radius() const { return radius_; }
    vector_t color() const { return col_; }

    void set_pos(const vector_t& new_pos) { pos_ = new_pos; }
    void set_radius(float new_radius) { radius_ = new_radius; }
    void set_col(const vector_t& new_col) { col_ = new_col; }

    bool is_intersection(float& distance, const ray_t& ray) const override;
    vector_t intersect(const ray_t& ray) const override;
};

//=============================================================================
class camera_t
{
private:
    vector_t pov_;

    axes_t   axes_;

public:
    camera_t(const vector_t& pov, const axes_t& axes)
        : pov_(pov), axes_(axes)
    {}

    void init_ray(ray_t& ray, point_t pixel) const;
};

//=============================================================================

class ray_t
{
private:
    const scene_t* parent_;
    
    int heir_depth_;

    vector_t pos_;
    vector_t unit_dir_;

    ray_t(const ray_t&) = delete;
    ray_t& operator=(const ray_t&) = delete;

public:
    ray_t(const ray_t& parent_ray, vector_t pos, vector_t dir)
        : parent_(parent_ray.parent_), heir_depth_(parent_ray.heir_depth_),
          pos_(pos), unit_dir_(dir / std::sqrt(dot(dir, dir)))
    {
        heir_depth_--;
    }

    ray_t(const scene_t* parent, int heir_depth)
        : parent_(parent), heir_depth_(heir_depth),
          pos_(vector_t()), unit_dir_(vector_t())
    {}

    ~ray_t() = default;

    vector_t pos() const { return pos_; }
    vector_t dir() const { return unit_dir_; }

    void set_pos(const vector_t& pos) { pos_ = pos; }
    void set_dir(const vector_t& dir) { unit_dir_ = dir / std::sqrt(dot(dir, dir)); }

    vector_t emit() const;
};

class scene_t
{
private:
    camera_t camera_;
    std::vector<const object_t*> objects_;

    // FIXME no difference in bckgrnd/ambient?
    vector_t bckgrnd_col_;
    vector_t ambient_col_;

    friend ray_t;

public:
    scene_t(const camera_t& cam,
            const vector_t& bckgrnd_col, const vector_t& ambient_col)
        : camera_(cam), objects_(),
          bckgrnd_col_(bckgrnd_col), ambient_col_(ambient_col)
    {}

    void add_object(const object_t& obj)
    {
        objects_.push_back(&obj);
    }

    vector_t eval_color(const point_t& point) const;
};

#endif // SCENE_H