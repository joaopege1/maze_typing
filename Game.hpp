#pragma once
#include <SFML/Graphics.hpp>
#include "player.hpp"
#include "Maze.hpp"
#include "constants.hpp"

class Game {
private:
    sf::RenderWindow window;
    GameState currentState;
    sf::Font font;
    sf::Clock gameClock;
    
    // O Jogo é "dono" do jogador e do labirinto
    Player player; 
    Maze maze;

    void processEvents();
    void update();
    void render();

public:
    Game();
    void run(); // O Game Loop (while window.isOpen) entra aqui
};
