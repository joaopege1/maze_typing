#include "player.hpp"

// Implementação do Construtor
Player::Player(float startX, float startY) {
    position = {startX, startY};
    shape.setSize({30.f, 30.f}); // 30x30 (um pouco menor que o bloco de 40)
    shape.setFillColor(sf::Color::Yellow);
    shape.setPosition(position);
}

// Implementação da Lógica de Movimento
void Player::move(sf::Vector2f offset) {
    position.x += offset.x;
    position.y += offset.y;
    shape.setPosition(position);
}

// Implementação do Desenho na Tela
void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}
