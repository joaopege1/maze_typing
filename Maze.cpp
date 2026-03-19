#include "Maze.hpp"

Maze::Maze() : grid({
    " ################### ",
    " S                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 # ",
    " #                 E ",
    " ################### ",
}) {}

char Maze::getTile(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return '#';
    if (x < 0 || x >= (int)grid[y].size()) return '#';
    return grid[y][x];
}

sf::Vector2f Maze::getStartPosition() const {
    for (size_t y = 0; y < grid.size(); y++)
        for (size_t x = 0; x < grid[y].size(); x++)
            if (grid[y][x] == 'S')
                return { (float)(x * TILE_SIZE) + 5.f, (float)(y * TILE_SIZE + 100) + 5.f };
    return { 0.f, 0.f };
}

void Maze::draw(sf::RenderWindow& window) const {
    for (size_t y = 0; y < grid.size(); y++) {
        for (size_t x = 0; x < grid[y].size(); x++) {
            char tile = grid[y][x];
            if (tile != '#' && tile != 'S' && tile != 'E') continue;

            sf::RectangleShape rect({ (float)TILE_SIZE, (float)TILE_SIZE });
            rect.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE + 100) });
            rect.setFillColor(tile == '#' ? sf::Color::White : sf::Color::Red);
            window.draw(rect);
        }
    }
}
