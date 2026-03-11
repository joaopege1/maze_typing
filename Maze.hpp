#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class Maze {
private:
    std::vector<std::string> grid;
public:
    Maze(); // Construtor onde você define a matriz
    void draw(sf::RenderWindow& window); // O for duplo vem pra cá
    char getTile(int x, int y); // Para checar colisões no futuro!
};
