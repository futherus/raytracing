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

vector_t axes_t::pixel2real(point_t pixel)
{
    assert(tl_pos_.x_ <= pixel.x_ && pixel.x_ <= br_pos_.x_);
    assert(tl_pos_.y_ <= pixel.y_ && pixel.y_ <= br_pos_.y_);

    float s11 =   (max_lim_.x_ - min_lim_.x_) / (float) (br_pos_.x_ - tl_pos_.x_);
    float s22 = - (max_lim_.y_ - min_lim_.y_) / (float) (br_pos_.y_ - tl_pos_.y_);

    pixel.x_ -= tl_pos_.x_;
    pixel.y_ -= tl_pos_.y_;
    vector_t origin(min_lim_.x_, max_lim_.y_, 0.f);

    return vector_t(s11 * (float) pixel.x_, s22 * (float) pixel.y_, 0.f) + origin;
}

// void axes_t::draw(const vector_t& vec, const vector_t& base)
// {
//     sf::VertexArray vec_line;
//     vec_line.setPrimitiveType(sf::Lines);
//     sf::VertexArray vec_arrow;
//     vec_arrow.setPrimitiveType(sf::Triangles);

//     sf::Vertex tail(real2pixel(base.x(), base.y()));
//     sf::Vertex head(real2pixel(vec.x() + base.x(), vec.y() + base.y()));

//     sf::Vector2f delta = (head.position - tail.position) * 0.15f;
//     sf::Vertex left_tip(sf::Vector2f(head.position.x - delta.x - delta.y / 2,
//                                      head.position.y - delta.y + delta.x / 2)
//                        );
    
//     sf::Vertex right_tip(sf::Vector2f(head.position.x - delta.x + delta.y / 2,
//                                       head.position.y - delta.y - delta.x / 2)
//                         );
    
//     vec_line.append(tail);
//     vec_line.append(head);

//     vec_arrow.append(head);
//     vec_arrow.append(left_tip);
//     vec_arrow.append(right_tip);

//     vertices_.push_back(vec_line);
//     vertices_.push_back(vec_arrow);
// }
