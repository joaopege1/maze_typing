#include "Maze.hpp"
#include <fstream>

void Maze::loadFromFile(const std::string& path) {
    grid.clear();
    std::ifstream file(path);
    std::string line;
    while (std::getline(file, line)) {
        auto first = line.find('"');
        auto last  = line.rfind('"');
        if (first != std::string::npos && last != std::string::npos && last > first)
            grid.push_back(line.substr(first + 1, last - first - 1));
    }
}

char Maze::getTile(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return '#';
    if (x < 0 || x >= (int)grid[y].size()) return '#';
    return grid[y][x];
}

bool Maze::isWall(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return true;
    if (x <= 0 || x >= (int)grid[y].size() - 1) return true;
    char tile = grid[y][x];
    return tile == '#' || tile == 'W' || tile == 'E' || tile == 'S';
}

bool Maze::isExit(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return false;
    if (x < 0 || x >= (int)grid[y].size()) return false;
    return grid[y][x] == 'E';
}

bool Maze::isStart(int x, int y) const {
    if (y < 0 || y >= (int)grid.size())    return false;
    if (x < 0 || x >= (int)grid[y].size()) return false;
    return grid[y][x] == 'S';
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

            if (tile == '#') {
                rect.setFillColor(sf::Color::White);
            } else if (tile == 'W') {
                rect.setFillColor(sf::Color::White);
            } else if (tile == 'S') {
                rect.setFillColor(sf::Color::Blue);
            } else if (tile == 'E') {
                rect.setFillColor(sf::Color::Green);
            } else {
                continue;
            }

            window.draw(rect);
        }
    }
}
