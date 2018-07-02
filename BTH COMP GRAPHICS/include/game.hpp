#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include "camera.hpp"

class game
{
public:
    game();
    ~game();

    void run();

private:
    static const unsigned int WIDTH = 1280;
    static const unsigned int HEIGHT = 720;

    sf::ContextSettings settings;
    sf::Window window;

    camera camera;
};

#endif // GAME_HPP
