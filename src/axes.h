#ifndef AXES_H
#define AXES_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "vector.h"

class axes_t : public sf::Drawable, public sf::Transformable
{
private:
    sf::Vector2f tl_pos_;
    sf::Vector2f br_pos_;

    sf::Vector2f min_lim_;
    sf::Vector2f max_lim_;

    std::vector<sf::VertexArray> vertices_;
    sf::VertexArray borders_;

public:
    axes_t(sf::Vector2f  tl_pos, sf::Vector2f  br_pos,
           sf::Vector2f min_lim, sf::Vector2f max_lim)
        : tl_pos_(tl_pos), br_pos_(br_pos), min_lim_(min_lim), max_lim_(max_lim),
          vertices_(), borders_() //, vec_(), arrow_()
    {
        borders_.append(sf::Vector2f(tl_pos.x, tl_pos.y));
        borders_.append(sf::Vector2f(tl_pos.x, br_pos.y));
        borders_.append(sf::Vector2f(br_pos.x, br_pos.y));
        borders_.append(sf::Vector2f(br_pos.x, tl_pos.y));
        borders_.append(sf::Vector2f(tl_pos.x, tl_pos.y));
    
        borders_.setPrimitiveType(sf::LineStrip);
    }

    void clear() { vertices_.clear(); }

    void draw(const vector_t& vec, const vector_t& base = vector_t(0, 0));

private:
    sf::Vector2f real2pixel(float x_real, float y_real);

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();

        // states.texture = &m_tileset;
        
        target.draw(borders_, states);

        sf::VertexArray vec_ = vertices_[1];

        for(size_t i = 0; i < vertices_.size(); i++)
        {
            target.draw(vertices_[i], states);
        }
    }
};

#endif // AXES_H
