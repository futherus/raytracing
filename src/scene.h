#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include "axes.h"
#include <cmath>

class scene_t;
class ray_t;

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

class ball_t
{
private:
    vector_t pos_;
    float radius_;

    vector_t col_;
    float glare_;

public:
    ball_t(const vector_t& pos, float radius, const vector_t& col, float glare)
        : pos_(pos), radius_(radius), col_(col), glare_(glare)
    {}

    ball_t(const ball_t& ball)
        : pos_(ball.pos_), radius_(ball.radius_), col_(ball.col_), glare_(ball.glare_)
    {}

    ball_t&
    operator=(const ball_t& ball)
    {
        pos_ = ball.pos_;
        radius_ = ball.radius_;
        col_ = ball.col_;
        glare_ = ball.glare_;

        return *this;
    }

    vector_t pos() const { return pos_; }
    float radius() const { return radius_; }
    vector_t col() const { return col_; }
    float glare()  const { return glare_; }

    void set_pos(const vector_t& new_pos) { pos_ = new_pos; }
    void set_radius(float new_radius) { radius_ = new_radius; }
    void set_col(const vector_t& new_col) { col_ = new_col; }
    void set_glare(float new_glare) { glare_ = new_glare; }

    /* virtual */ bool collision(vector_t& col, ray_t ray) const;
};

class light_t
{
private:
    vector_t pos_;
    float radius_;

    vector_t col_;

public:
    light_t(const vector_t& pos, float radius, const vector_t& col)
        : pos_(pos), radius_(radius), col_(col)
    {}

    light_t(const light_t& light)
        : pos_(light.pos_), radius_(light.radius_), col_(light.col_)
    {}

    light_t& operator=(const light_t& light)
    {
        pos_    = light.pos_;
        radius_ = light.radius_;
        col_    = light.col_;

        return *this;
    }

    vector_t pos() const { return pos_; }
    float radius() const { return radius_; }
    vector_t col() const { return col_; }

    void set_pos(const vector_t& new_pos) { pos_ = new_pos; }
    void set_radius(float new_radius) { radius_ = new_radius; }
    void set_col(const vector_t& new_col) { col_ = new_col; }

    /* virtual */ bool collision(vector_t& col, ray_t ray) const;
};

class ray_t
{
private:
    const scene_t* parent_;
    
    int heir_depth_;

    vector_t pos_;
    vector_t unit_;

public:
    ray_t(const ray_t& parent_ray, vector_t pos, vector_t dir)
        : parent_(parent_ray.parent_), heir_depth_(parent_ray.heir_depth_),
          pos_(pos), unit_(dir * std::sqrt(dot(dir, dir)))
    {
        heir_depth_--;
    }

    ray_t(const scene_t* parent, int heir_depth)
        : parent_(parent), heir_depth_(heir_depth),
          pos_(vector_t()), unit_(vector_t())
    {}

    vector_t pos() const { return pos_; }
    vector_t dir() const { return unit_; }

    void set_pos(const vector_t& pos) { pos_ = pos; }
    void set_dir(const vector_t& dir) { unit_ = dir * std::sqrt(dot(dir, dir)); }

    vector_t emit() const;
};

class scene_t
{
private:
    camera_t camera_;
    ball_t ball_;
    light_t light_;

    vector_t bckgrnd_col_;
    vector_t ambient_col_;

    friend ray_t;

public:
    scene_t(const camera_t& cam, const ball_t& ball, const light_t& light,
            const vector_t& bckgrnd_col, const vector_t& ambient_col)
        : camera_(cam), ball_(ball), light_(light),
          bckgrnd_col_(bckgrnd_col), ambient_col_(ambient_col)
    {}

    void set_light(const light_t& new_light)
    {
        light_ = new_light;
    }

    void set_ball(const ball_t& new_ball)
    {
        ball_ = new_ball;
    }

    vector_t eval_color(const point_t& point) const;
};

#endif // SCENE_H