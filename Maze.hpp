#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "constants.hpp"

class Maze {
public:
    Maze();

    // Returns the tile character at grid position (x, y)
    char getTile(int x, int y) const;

    // Returns world position of the 'S' tile
    sf::Vector2f getStartPosition() const;

    void draw(sf::RenderWindow& window) const;

private:
    std::vector<std::string> grid;
};
