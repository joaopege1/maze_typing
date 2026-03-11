#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <string>
#include <optional>
#include <iostream>

#include "player.hpp"

using namespace std;

// Os estados do nosso jogo
enum GameState { MENU, PLAYING, GAME_OVER };

vector<string> maze = {
    " ################### ",
    " S   WWWWWWWWW     # ",
    " #       W         # ",
    " #       W         # ",
    " #   W   W         # ",
    " #   WWWWWWWWW     # ",
    " #       W   W     # ",
    " #       W   W     # ",
    " #       WWWWW     # ",
    " #       W         # ",
    " #       W         E ",
    " ################### ",
};

// Configurações da Tela
const unsigned int SCREEN_WIDTH = 850;
const unsigned int SCREEN_HEIGHT = 600;
const int SCREEN_CENTER_X = SCREEN_WIDTH  / 2;
const int SCREEN_CENTER_Y = SCREEN_HEIGHT / 2;
const int TILE_SIZE = 40; // Tamanho de cada bloco do labirinto

int main() {
    sf::RenderWindow window(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}), "Speed Typing Maze 2D");
    window.setFramerateLimit(60);

    // 1. Carregando a Fonte
    sf::Font font;
    if (!font.openFromFile("game_font.ttf")) {
        cout << "ERRO: Arquivo de fonte 'game_font.ttf' não encontrado na pasta!" << endl;
        return -1; 
    }

    // 2. Criando os Textos
    sf::Text titleText(font);
    titleText.setString("SPEED TYPING MAZE");
    titleText.setCharacterSize(60);
    titleText.setFillColor(sf::Color::Yellow);
    titleText.setPosition({SCREEN_CENTER_X/3.0f, SCREEN_CENTER_Y/2.0f}); 

    sf::Text startText(font);
    startText.setString("Press ENTER to start");
    startText.setCharacterSize(30);
    startText.setFillColor(sf::Color::White);
    startText.setPosition({250.f, 300.f});

    sf::Text timerText(font);
    timerText.setCharacterSize(40);
    timerText.setFillColor(sf::Color::Red);
    timerText.setPosition({30.f, 30.f});

    sf::Text gameOver(font);
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(60);
    gameOver.setFillColor(sf::Color::Red);
    gameOver.setPosition({SCREEN_CENTER_X/1.6f, SCREEN_CENTER_Y/2.0f});

    sf::Text gameOverText(font);
    gameOverText.setString("Press ENTER to restart or ESC to exit");
    gameOverText.setCharacterSize(30);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setPosition({SCREEN_CENTER_X/3, SCREEN_CENTER_Y});

    GameState currentState = MENU;
    sf::Clock gameClock; 
    const float TIME_LIMIT = 10.0f;

    // --- CÁLCULO DA POSIÇÃO INICIAL DO JOGADOR ---
    float playerStartX = 0.0f;
    float playerStartY = 0.0f;

    for (int y = 0; y < maze.size(); y++) {
        for (int x = 0; x < maze[y].size(); x++) {
            if (maze[y][x] == 'S') {
                // Acha a posição do S em pixels.
                // Somamos +5 para centralizar o jogador (30x30) no bloco (40x40)
                playerStartX = (float)(x * TILE_SIZE) + 5.0f; 
                playerStartY = (float)(y * TILE_SIZE + 100) + 5.0f;
            }
        }
    }

    // Instancia o jogador passando as coordenadas que encontramos
    Player meuJogador(playerStartX, playerStartY);

    // Loop Principal
    while (window.isOpen()) {
        
        // --- 1. PROCESSAR EVENTOS ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (currentState == MENU) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        currentState = PLAYING;
                        gameClock.restart(); 
                    }
                } 
                else if (currentState == GAME_OVER) {
                    if (keyEvent->code == sf::Keyboard::Key::Enter) {
                        currentState = PLAYING;
                        gameClock.restart(); 
                        // Opcional futuro: resetar a posição do jogador aqui!
                    } else if (keyEvent->code == sf::Keyboard::Key::Escape) {
                        window.close();
                    }
                }
            }
        }

        // --- 2. LÓGICA (UPDATE) ---
        if (currentState == PLAYING) {
            float timeElapsed = gameClock.getElapsedTime().asSeconds();
            float timeLeft = TIME_LIMIT - timeElapsed;

            if (timeLeft <= 0) {
                timeLeft = 0;
                currentState = GAME_OVER;
            }
            timerText.setString("Time: " + to_string((int)timeLeft) + "s");
        }

        // --- 3. DESENHAR NA TELA (DRAW) ---
        window.clear(sf::Color::Black);

        if (currentState == MENU) {
            window.draw(titleText);
            window.draw(startText);
        } 
        else if (currentState == PLAYING) {
            // Desenha o Labirinto
            for (int y = 0; y < maze.size(); y++) {
                for (int x = 0; x < maze[y].size(); x++) {
                    sf::RectangleShape tile(sf::Vector2f({(float)TILE_SIZE, (float)TILE_SIZE}));
                    tile.setPosition({(float)(x * TILE_SIZE), (float)(y * TILE_SIZE + 100)});

                    if (maze[y][x] == '#') {
                        tile.setFillColor(sf::Color::White);
                    } else if (maze[y][x] == 'W') {
                        tile.setFillColor(sf::Color::Blue);
                    } else if (maze[y][x] == 'S' || maze[y][x] == 'E') {
                        tile.setFillColor(sf::Color::Red);
                    } else {
                        continue; 
                    }
                    window.draw(tile);
                }
            }
            
            // ---> DESENHA O JOGADOR AQUI (por cima do labirinto) <---
            meuJogador.draw(window);

            // Desenha o Timer
            window.draw(timerText);
        }
        else if (currentState == GAME_OVER) {
            window.draw(gameOver);
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}
