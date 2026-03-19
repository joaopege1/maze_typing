#include "Player.hpp"
#include "constants.hpp"

Player::Player(float startX, float startY)
{
    position = {startX, startY};

    shape.setSize({30.f, 30.f});
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
}

void Player::move(sf::Vector2f offset, const Maze& maze)
{
    sf::Vector2f futurePosition = position + offset;

    // Converte pixels -> grid para verificar colisão
    int gridX = (int)(futurePosition.x / TILE_SIZE);
    int gridY = (int)((futurePosition.y - 100) / TILE_SIZE);

    if (!maze.isWall(gridX, gridY)) {
        position = futurePosition;
        shape.setPosition(position);
    }
}

void Player::draw(sf::RenderWindow& window)
{
    window.draw(shape);
}

void Player::setPosition(float x, float y)
{
    position = {x, y};
    shape.setPosition(position);
}

sf::Vector2f Player::getPosition() const
{
    return position;
}
