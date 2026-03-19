#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "constants.hpp"

class Maze {
public:
    Maze();

    char getTile(int x, int y) const;
    bool isWall(int x, int y) const;
    bool isExit(int x, int y) const;
    bool isStart(int x, int y) const;

    sf::Vector2f getStartPosition() const;

    void draw(sf::RenderWindow& window) const;

private:
    std::vector<std::string> grid;
};
