#ifndef SCENE_H
#define SCENE_H

#include "vector.h"

class ball_t
{
private:
    vector_t pos_;
    float radius_;

    vector_t col_;
    // int glare_n;

public:
    ball_t(const vector_t& pos, float radius, const vector_t& col)
        : pos_(pos), radius_(radius), col_(col)
    {}

    ball_t(const ball_t& ball)
        : pos_(ball.pos_), radius_(ball.radius_), col_(ball.col_)
    {}

    ball_t& operator=(const ball_t& ball)
    {
        pos_ = ball.pos_;
        radius_ = ball.radius_;
        col_ = ball.col_;

        return *this;
    }

    vector_t pos() const { return pos_; }
    float radius() const { return radius_; }
    vector_t col() const { return col_; }
};

class light_t
{
private:
    vector_t dir_;

    vector_t col_;

public:
    light_t(const vector_t& dir, const vector_t& col)
        : dir_(dir), col_(col)
    {}

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

    vector_t eval_color(const vector_t& point);
};

#endif // SCENE_H