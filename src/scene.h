#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "axes.h"
#include <cmath>
#include <vector>
#include <array>

class scene_t;
class ray_t;

//=============================================================================
class material_t final
{
private:
    std::vector<vector_t>   dirs_;
    std::array<vector_t, 3> basis_;

    vector_t glow_;
    vector_t color_;

public:
    material_t(float (*distr_func)(float angle), int dirs_count,
               vector_t glow, vector_t color);

    material_t(const material_t&)            = default;
    material_t& operator=(const material_t&) = default;
    ~material_t()                            = default;

    void set_basis(const vector_t& normal);

    vector_t dir(size_t index) const;
    size_t n_dir() const { return dirs_.size(); }
    float weight() const { return 1 / (float) n_dir(); }

    const vector_t& glow()  const { return glow_; }
    const vector_t& color() const { return color_; }
    void set_glow (const vector_t& new_glow)  { glow_ = new_glow; }
    void set_color(const vector_t& new_color) { color_ = new_color; }
};

class shape_t
{
private:
    vector_t pos_;

public:
    shape_t(const vector_t& pos)
        : pos_(pos)
    {}

    shape_t(const shape_t&)            = default;
    shape_t& operator=(const shape_t&) = default;
    virtual ~shape_t()                 = 0;

    const vector_t& pos() const { return pos_; }
    void set_pos(const vector_t& new_pos) { pos_ = new_pos; }

    virtual shape_t* clone() const = 0;
    virtual bool get_intersection(float& distance, vector_t& point, vector_t& normal,
                                  const ray_t& ray) const = 0;
};

class object_t final
{
private:
    shape_t*    shape_;
    material_t* material_;

public:
    object_t(const shape_t& shape, const material_t& mat)
        : shape_(shape.clone()), material_(new material_t(mat))
    {}

    object_t(const object_t&) = delete;
    object_t& operator=(const object_t&) = delete;

    ~object_t()
    {
        delete material_;
        delete shape_;
    }

    void set_shape   (const shape_t& shape)       { *shape_    = shape; }
    void set_material(const material_t& material) { *material_ = material; }

    bool get_intersection(float& distance, vector_t& point, vector_t& normal,
                          const ray_t& ray) const;

    vector_t intersect(const vector_t& point, const vector_t& normal, const ray_t& ray) const;
};

//=============================================================================

class ball_t : public shape_t
{
private:
    float radius_;

public:
    ball_t(const ball_t& ball)            = default;
    ball_t& operator=(const ball_t& ball) = default;
    ~ball_t()                             = default;

    ball_t(const vector_t& pos, float radius)
        : shape_t(pos), radius_(radius)
    {}

    float radius() const { return radius_; }
    void set_radius(float new_radius) { radius_ = new_radius; }

    bool get_intersection(float& dist, vector_t& point, vector_t& normal,
                          const ray_t& ray) const override;

    shape_t* clone() const override { return new ball_t(*this); }
};

/*
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
*/

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