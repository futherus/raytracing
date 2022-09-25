#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <cmath>
#include "axes.h"
#include "graphics.h"
#include "scene.h"

const uint32_t WINDOW_WIDTH  = 1600;
const uint32_t WINDOW_HEIGHT = 1200;

const uint32_t SCENE_WIDTH = 600;
const uint32_t SCENE_HEIGHT = 600;

constexpr vector_t from_rgb(float r, float g, float b)
{
    return vector_t(r / 255.f, g / 255.f, b / 255.f);
}

const vector_t COLOR_RED = from_rgb(255.f, 0.f, 0.f);
const vector_t COLOR_GREEN = from_rgb(0.f, 255.f, 0.f);
const vector_t COLOR_BLUE = from_rgb(0.f, 0.f, 255.f);
const vector_t COLOR_SLATEGRAY = from_rgb(112.f, 128.f, 144.f);
const vector_t COLOR_DARKSLATEGRAY = from_rgb(47.f, 79.f, 79.f);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                            "My window name", sf::Style::Default);
    
    sf::Clock clock;

    sf::Texture texture;
    texture.create(SCENE_WIDTH, SCENE_HEIGHT);
    sf::Sprite sprite(texture);
    uint32_t* pixels = new uint32_t[SCENE_WIDTH * SCENE_HEIGHT];
    
    ball_t ball({0.5f, 0.f, 0.f}, 0.5f, COLOR_BLUE);
    light_t light({1.f, 1.f, 0.f}, COLOR_BLUE);
    scene_t scene(ball, light, COLOR_DARKSLATEGRAY, 0.15f * COLOR_BLUE);

    axes_t axes({0, 0}, {SCENE_WIDTH, SCENE_HEIGHT},
                {-1, -1, 0}, {1, 1, 0}
               );

    while(window.isOpen())
    {
        sf::Event event = {};

        while(window.pollEvent(event))
        {
            switch(event.type)
            {
                case sf::Event::Closed:
                {
                    window.close();
                    break;
                }
                default:
                {
                    // fall through
                    void(0);
                }
            }
        }

        sf::Time elapsed = clock.restart();

        for (uint32_t y_pos = 0; y_pos < SCENE_HEIGHT; y_pos++)
        {
            for (uint32_t x_pos = 0; x_pos < SCENE_WIDTH; x_pos++)
            {
                vector_t real = axes.pixel2real({x_pos, y_pos});
                vector_t color = scene.eval_color(real);
                set_pixel(pixels[y_pos * SCENE_WIDTH + x_pos], color);
            }
        }

        window.clear();
        texture.update((sf::Uint8*) pixels);

        window.draw(sprite);
        window.display();
    }

    return 0;
}
