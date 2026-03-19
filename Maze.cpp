#include "Maze.hpp"

Maze::Maze() : grid({
    " ################### ",
    " S W               # ",
    " # W               # ",
    " # W               # ",
    " # W               # ",
    " # W               # ",
    " # W               # ",
    " # WWWWWWWWWWWWWWWW# ",
    " #                 E ",
    " ################### ",
}) {}

char Maze::getTile(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return '#';
    if (x < 0 || x >= (int)grid[y].size()) return '#';
    return grid[y][x];
}

bool Maze::isWall(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return true;
    if (x < 0 || x >= (int)grid[y].size()) return true;
    char tile = grid[y][x];
    return tile == '#' || tile == 'W' || tile == 'E';
}

bool Maze::isExit(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return false;
    if (x < 0 || x >= (int)grid[y].size()) return false;
    char tile = grid[y][x];
    return tile == 'E';
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

            sf::RectangleShape rect({ (float)TILE_SIZE, (float)TILE_SIZE });
            rect.setPosition({ (float)(x * TILE_SIZE), (float)(y * TILE_SIZE + 100) });

            if (tile == '#') { // maze grid
                rect.setFillColor(sf::Color::White);
            } else if (tile == 'W') { // maze wall
                rect.setFillColor(sf::Color::White);
            } else if (tile == 'S') { // start
                rect.setFillColor(sf::Color::Red);
            } else if (tile == 'E') { // exit, end
                rect.setFillColor(sf::Color::Red);
            } else {
                continue;
            }

            window.draw(rect);
        }
    }
}
