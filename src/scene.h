#ifndef SCENE_H
#define SCENE_H

#include "vector.h"
#include <cmath>

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

    ball_t& operator=(const ball_t& ball)
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
};

class light_t
{
private:
    vector_t dir_;

    vector_t col_;

public:
    light_t(vector_t dir, const vector_t& col)
        : col_(col)
    {
        // normalise vector of light direction
        dir_ = dir / std::sqrt(dot(dir, dir));
    }

    light_t(const light_t& light)
        : dir_(light.dir_), col_(light.col_)
    {}

    light_t& operator=(const light_t& light)
    {
        dir_ = light.dir_;
        col_ = light.col_;

        return *this;
    }

    vector_t dir() const { return dir_; }
    vector_t col() const { return col_; }

    void set_col(const vector_t& new_col) { col_ = new_col; }
    void set_dir(const vector_t& new_dir) { dir_ = new_dir; }
};

class scene_t
{
private:
    ball_t ball_;
    light_t light_;

    vector_t bckgrnd_col_;
    vector_t ambient_col_;

public:
    scene_t(const ball_t& ball, const light_t& light,
            const vector_t& bckgrnd_col, const vector_t& ambient_col)
        : ball_(ball), light_(light), bckgrnd_col_(bckgrnd_col), ambient_col_(ambient_col)
    {}

    void set_light(const light_t& new_light)
    {
        light_ = new_light;
    }

    void set_ball(const ball_t& new_ball)
    {
        ball_ = new_ball;
    }

    vector_t eval_color(const vector_t& point);
};

#endif // SCENE_H