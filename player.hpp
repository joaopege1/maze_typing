#pragma once
#include <SFML/Graphics.hpp>
#include "Maze.hpp"

class Player
{
private:
    sf::Vector2f position;
    sf::RectangleShape shape;

public:
    Player(float startX, float startY);

    void move(sf::Vector2f offset, const Maze& maze);
    void draw(sf::RenderWindow& window);

    void setPosition(float x, float y);
    sf::Vector2f getPosition() const;
};
