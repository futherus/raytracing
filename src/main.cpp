#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <cmath>
#include "axes.h"
#include "graphics.h"
#include "scene.h"

const uint32_t WINDOW_WIDTH  = 1920;
const uint32_t WINDOW_HEIGHT = 1080;

const uint32_t SCENE_WIDTH = 1920;
const uint32_t SCENE_HEIGHT = 1080;

constexpr vector_t from_rgb(float r, float g, float b)
{
    return vector_t(r / 255.f, g / 255.f, b / 255.f);
}

const vector_t COLOR_WHITE = from_rgb(255.f, 255.f, 255.f);
const vector_t COLOR_LIGHTYELLOW = from_rgb(255.f, 255.f, 102.f);
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
    
    axes_t axes({SCENE_WIDTH, SCENE_HEIGHT},
                {-20.f, 12.f,  0.f},
                { 40.f,  0.f,  0.f},
                { 0.f, -24.f,  0.f}
                );
    std::cout << axes.pixel2real({0, 0}) << " "
              << axes.pixel2real({SCENE_WIDTH, SCENE_HEIGHT}) << std::endl;
    camera_t cam({0.f, 0.f, 70.f}, axes);

    ball_t   ball({0.f, 0.f, -20.f}, 5.f, COLOR_SLATEGRAY, 15.f);
    light_t  light({0.f, 0.f, 0.f}, 3.f, COLOR_WHITE);
    scene_t  scene(cam, ball, light, COLOR_DARKSLATEGRAY, 0.15f * COLOR_RED);

    ray_t ray(&scene, 3);
    cam.init_ray(ray, {0, 0});
    std::cout << "Ray: " << ray.pos() << " " << ray.dir() << std::endl;

    float angle = 0;
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
        // std::cout << (1 / elapsed.asSeconds()) << std::endl;
        angle += elapsed.asSeconds();

        light.set_pos({12.f * std::cos(angle), 12.f * std::sin(angle), -20.f});
        scene.set_light(light);

        for (uint32_t y_pos = 0; y_pos < SCENE_HEIGHT; y_pos++)
        {
            for (uint32_t x_pos = 0; x_pos < SCENE_WIDTH; x_pos++)
            {
                vector_t color = scene.eval_color({x_pos, y_pos});
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
