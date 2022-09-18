#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <cmath>
#include "axes.h"

const float CONST_PI = 3.14159;

int main()
{
    axes_t axes1(sf::Vector2f(100, 100), sf::Vector2f(400, 400),
                 sf::Vector2f(-1, -1),   sf::Vector2f(1, 1));

    axes_t axes2(sf::Vector2f(500, 100), sf::Vector2f(800, 400),
                 sf::Vector2f(-1, -1),   sf::Vector2f(1, 1));

    axes_t stopwatch_axes(sf::Vector2f(900, 100), sf::Vector2f(1200, 400),
                          sf::Vector2f(-1, -1),   sf::Vector2f(1, 1));

    sf::Clock stopwatch;

    float angle  = 0;
    float length = 0.5;

    sf::RenderWindow window(sf::VideoMode(1600, 1200), "My window name", sf::Style::Default);
    sf::Clock clock;

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

        angle += elapsed.asSeconds();
        vector_t vec (length * std::cos(angle), length * std::sin(angle));
        vector_t base(length * std::cos(angle * 2), length * std::sin(angle * 2));
        float secs = CONST_PI / 2 - stopwatch.getElapsedTime().asSeconds() * 2 * CONST_PI / 60;
        vector_t stopwatch_sec(length * std::cos(secs),
                               length * std::sin(secs));
        float mins = CONST_PI / 2 - stopwatch.getElapsedTime().asSeconds() * 2 * CONST_PI / 360;
        vector_t stopwatch_mins(length * 0.8 * std::cos(mins),
                                length * 0.8 * std::sin(mins));

        window.clear();
        axes1.clear();
        axes2.clear();
        stopwatch_axes.clear();

        axes1.draw(vec, base);
        axes1.draw(base);
        axes1.draw(base + vec);
        axes2.draw(vec);
        stopwatch_axes.draw(stopwatch_sec);
        stopwatch_axes.draw(stopwatch_mins);

        window.draw(axes1);
        window.draw(axes2);
        window.draw(stopwatch_axes);

        window.display();
    }

    return 0;
}
