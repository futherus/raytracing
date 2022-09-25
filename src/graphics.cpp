#include <SFML/Graphics.hpp>

#include "graphics.h"

void set_pixel(uint32_t& pixel, const vector_t& color)
{
    uint8_t* ptr = (uint8_t*) &pixel;

    ptr[0] = 255 * color.x_;
    ptr[1] = 255 * color.y_;
    ptr[2] = 255 * color.z_;
    ptr[3] = 255;
}
