#include "axes.h"

sf::Vector2f axes_t::real2pixel(float x_real, float y_real)
{
    float s11 =   (br_pos_.x - tl_pos_.x) / (max_lim_.x - min_lim_.x);
    float s22 = - (br_pos_.y - tl_pos_.y) / (max_lim_.y - min_lim_.y);

    sf::Vector2f origin(-min_lim_.x * s11, -max_lim_.y * s22);

    return sf::Vector2f(s11 * x_real, s22 * y_real) + origin + tl_pos_;
}

void axes_t::draw(const vector_t& vec, const vector_t& base)
{
    sf::VertexArray vec_line;
    vec_line.setPrimitiveType(sf::Lines);
    sf::VertexArray vec_arrow;
    vec_arrow.setPrimitiveType(sf::Triangles);

    sf::Vertex tail(real2pixel(base.x(), base.y()));
    sf::Vertex head(real2pixel(vec.x() + base.x(), vec.y() + base.y()));

    sf::Vector2f delta = (head.position - tail.position) * 0.15f;
    sf::Vertex left_tip(sf::Vector2f(head.position.x - delta.x - delta.y / 2,
                                     head.position.y - delta.y + delta.x / 2)
                       );
    
    sf::Vertex right_tip(sf::Vector2f(head.position.x - delta.x + delta.y / 2,
                                      head.position.y - delta.y - delta.x / 2)
                        );
    
    vec_line.append(tail);
    vec_line.append(head);

    vec_arrow.append(head);
    vec_arrow.append(left_tip);
    vec_arrow.append(right_tip);

    vertices_.push_back(vec_line);
    vertices_.push_back(vec_arrow);
}
