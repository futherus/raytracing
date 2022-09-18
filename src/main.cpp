#include <SFML/Graphics.hpp>
#include <unistd.h>
#include "axes.h"

int main()
{
    vector_t vec(0.5, 0.5);

    axes_t axes(sf::Vector2f(100, 100), sf::Vector2f(400, 400),
                sf::Vector2f(-1, -1),   sf::Vector2f(1, 1));

    axes.draw(vec);

    sf::RenderWindow window(sf::VideoMode(800, 600), "My window name", sf::Style::Default);

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
                // case sf::Event::KeyPressed:
                // {
                //     switch(event.key.code)
                //     {
                //         case sf::Keyboard::Up:
                //         {
                //             add_balls(&data, 1);
                //             break;
                //         }
                //         case sf::Keyboard::Down:
                //         {
                //             remove_balls(&data, 1);
                //             break;
                //         }
                //         case sf::Keyboard::T:
                //         {
                //             printf("T: %f\n", evaluate_temperature(&data));
                //             break;
                //         }
                //         default:
                //         {
                //             void(0);
                //             // fall through
                //         }
                //     }
                //     break;
                // }
                default:
                {
                    // fall through
                    void(0);
                }
            }
        }

        // sf::Time elapsed = clock.restart();

        window.clear();

        axes.clear();
        axes.draw(vec);
        window.draw(axes);

        window.display();
    }

    return 0;
}
