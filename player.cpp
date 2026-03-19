#include "player.hpp"

Player::Player(float startX, float startY)
{
    position = {startX, startY};

    shape.setSize({30.f, 30.f});
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(position);
}

void Player::move(sf::Vector2f offset)
{
    position.x += offset.x;
    position.y += offset.y;

    shape.setPosition(position);
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
