#pragma once // Isso impede que o arquivo seja lido duas vezes pelo compilador
#include <SFML/Graphics.hpp>

class Player {
private:
    // Variáveis que só o Player pode mexer
    sf::Vector2f position;
    sf::RectangleShape shape;

public:
    // Construtor (chamado quando o Player nasce)
    Player(float startX, float startY);

    // Funções (Ações que o Player pode fazer)
    void move(sf::Vector2f offset);
    void draw(sf::RenderWindow& window);
};
